#!/bin/bash
# Simple Minishell Tester - Runs Commands in Bash & Minishell
#
# This script runs commands in both Bash and your Minishell,
# compares their outputs and exit codes, and shows any differences.
#
# Usage:
#  Interactive: ./tester.sh
#  File mode:   ./tester.sh commands.txt (runs commands from the file)
#
# Requirements:
#  - A compiled "minishell" executable in the same directory.
#  - Bash 4+ (for coprocess support).


if [[ "${BASH_SOURCE[0]}" != "$0" ]]; then
    echo "Please run this script directly (e.g. ./tester.sh), do not source it."
    return 1
fi


MINISHELL="./minishell"       
BASH_BIN="/bin/bash"        


if [[ ! -x "$MINISHELL" ]]; then
    echo "Error: Minishell executable '$MINISHELL' not found or not executable."
    exit 1
fi


RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
NC='\033[0m' 


set -o emacs


total_tests=0
passed_tests=0
failed_tests=0


BASH_PROC=()        
MS=()              
BASH_PROC_PID=""   
MS_PID=""          


cleanup() {
    if [[ -n "$BASH_PROC_PID" ]]; then
        kill "$BASH_PROC_PID" 2>/dev/null
        wait "$BASH_PROC_PID" 2>/dev/null
    fi
    if [[ -n "$MS_PID" ]]; then
        kill "$MS_PID" 2>/dev/null
        wait "$MS_PID" 2>/dev/null
    fi
    [[ -n "${BASH_PROC[0]}" ]] && exec {BASH_PROC[0]}>&-
    [[ -n "${BASH_PROC[1]}" ]] && exec {BASH_PROC[1]}>&-
    [[ -n "${MS[0]}" ]] && exec {MS[0]}>&-
    [[ -n "${MS[1]}" ]] && exec {MS[1]}>&-
}
trap cleanup EXIT

start_coprocesses() {
    coproc BASH_PROC { stdbuf -o0 "$BASH_BIN" --norc --noprofile; }
    BASH_PROC_PID=$!
    coproc MS { stdbuf -o0 "$MINISHELL"; }
    MS_PID=$!
    sleep 0.5
    while read -t 0.1 -r _ <&"${BASH_PROC[0]}"; do :; done
    while read -t 0.1 -r _ <&"${MS[0]}"; do :; done
}


run_test() {
    local cmd="$1"
    if [[ -z "$cmd" ]]; then
        return
    fi

    ((total_tests++))
    echo -e "${CYAN}\nRunning command: ${WHITE}${cmd}${NC}"
    echo "$cmd" >&"${BASH_PROC[1]}"
    echo "echo __BASH_EXIT_CODE:\$?" >&"${BASH_PROC[1]}"
    echo "echo __BASH_END__" >&"${BASH_PROC[1]}"

    local bash_output=""
    local bash_exit=""
    while IFS= read -r line <&"${BASH_PROC[0]}"; do
        if [[ "$line" == "__BASH_END__" ]]; then
            break
        elif [[ "$line" =~ ^__BASH_EXIT_CODE:([0-9]+)$ ]]; then
            bash_exit="${BASH_REMATCH[1]}"
        else
            bash_output+="$line"$'\n'
        fi
    done

    echo "$cmd" >&"${MS[1]}"
    echo "echo __MINISHELL_EXIT_CODE:\$?" >&"${MS[1]}"
    echo "echo __MINISHELL_END__" >&"${MS[1]}"

    local minishell_output=""
    local minishell_exit=""
    local prompt_regex='^[^[:space:]]+>.*'
    while IFS= read -r line <&"${MS[0]}"; do
        if [[ "$line" =~ $prompt_regex ]]; then
            continue
        fi

        if [[ "$line" == "__MINISHELL_END__" ]]; then
            break
        elif [[ "$line" =~ ^__MINISHELL_EXIT_CODE:([0-9]+)$ ]]; then
            minishell_exit="${BASH_REMATCH[1]}"
        else
            minishell_output+="$line"$'\n'
        fi
    done


    minishell_output="$(echo -n "$minishell_output" | sed "1{/^$cmd$/d}")"


    bash_output="$(echo -n "$bash_output" | sed -E ':a; /./!d; s/\n$//')"
    minishell_output="$(echo -n "$minishell_output" | sed -E ':a; /./!d; s/\n$//')"


    echo -e "\n${WHITE}[Bash Output]:${NC}"
    echo -e "${GREEN}${bash_output}${NC}"
    echo -e "${WHITE}[Exit Code]:${NC} ${CYAN}${bash_exit}${NC}"

    echo -e "\n${WHITE}[Minishell Output]:${NC}"
    echo -e "${YELLOW}${minishell_output}${NC}"
    echo -e "${WHITE}[Exit Code]:${NC} ${CYAN}${minishell_exit}${NC}"


    if [[ "$bash_output" == "$minishell_output" && "$bash_exit" == "$minishell_exit" ]]; then
        echo -e "${GREEN}✅ Outputs Match!${NC}"
        ((passed_tests++))
    else
        echo -e "${RED}❌ Differences Detected!${NC}"
        local tmp_bash tmp_ms
        tmp_bash=$(mktemp)
        tmp_ms=$(mktemp)
        echo "$bash_output"      > "$tmp_bash"
        echo "$minishell_output" > "$tmp_ms"
        echo -e "${WHITE}🔍 Diff between Bash and Minishell:${NC}"
        diff --color=always -u "$tmp_bash" "$tmp_ms" | tail -n +3
        rm "$tmp_bash" "$tmp_ms"
        ((failed_tests++))
    fi
    echo -e "${CYAN}-----------------------------${NC}\n"
}


start_coprocesses

if [[ $# -ge 1 ]]; then

    file="$1"
    if [[ ! -f "$file" ]]; then
        echo -e "${RED}Error: File '$file' not found.${NC}"
        exit 1
    fi
    echo -e "${CYAN}Running in File Mode. Reading commands from: ${WHITE}$file${NC}"

    while IFS= read -r line || [[ -n "$line" ]]; do
        [[ "$line" =~ ^#.*$ ]] && continue
        if [[ -n "$line" ]]; then
            run_test "$line"
        fi
    done < "$file"
    echo -e "${CYAN}===== Test Summary =====${NC}"
    echo -e "${WHITE}Total Commands: ${NC}$total_tests"
    echo -e "${GREEN}Passed: ${NC}$passed_tests"
    echo -e "${RED}Failed: ${NC}$failed_tests"
    echo -e "${CYAN}========================${NC}"
else
    echo -e "${CYAN}Interactive Minishell Mirror Tester Started!${NC}"
    echo -e "${WHITE}Enter commands to run them in both Bash and Minishell.${NC}"
    echo -e "${YELLOW}Type 'exit' to quit.${NC}"
    if [ -t 0 ]; then
        history -c
        HISTFILE=/dev/null
    fi

    while true; do
        prompt=$(echo -e "${GREEN}[Bash] \$ ${NC}")
        read -e -r -p "$prompt" cmd

        if [[ $? -ne 0 ]]; then
            echo -e "\n${RED}No input; exiting.${NC}"
            break
        fi
        if [[ -n "$cmd" ]]; then
            history -s "$cmd"
        fi
        if [[ "$cmd" == "exit" ]]; then
            echo -e "${YELLOW}Exiting...${NC}"
            echo "exit" >&"${BASH_PROC[1]}"
            echo "exit" >&"${MS[1]}"
            break
        fi
        run_test "$cmd"
    done
fi
echo -e "${WHITE}Mirror session ended.${NC}"
exit 0
