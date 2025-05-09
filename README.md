# 🐚 Minishell — A Simple Linux Shell Interpreter

Minishell is a custom Linux shell that supports a wide range of basic functionalities. It's stable, memory-safe, and crafted to follow POSIX conventions as closely as possible. Built as part of the 42 curriculum, this shell supports:

### ✅ Features

* ✅ Execute standard Linux commands by resolving them from the `$PATH`
* ✅ Run any binary using **relative** or **absolute** paths
* ✅ Handle **environment variable expansion** and **quoting**
* ✅ Support for **piping** multiple commands using the `|` operator
* ✅ Robust **I/O redirection**:

  * `>` redirect output
  * `<` redirect input
  * `>>` append to a file
  * `<<` heredoc support

---

## ⚙️ Build and Usage

```bash
# Clone the repository
git clone https://github.com/LinuxHeads/minishell.git
cd minishell

# Build the shell
make

# Launch the shell
./minishell
```

Example usage:

```bash
shell> ls -l
shell> echo $USER
shell> cat file.txt | grep "42"
```

---

## 📌 Examples

```bash
shell> echo Hello, world!
Hello, world!

shell> cat < minishell.h | grep doesnotexist

shell> echo $HOME
/home/your-username
```

---

## ⚠️ Known Issues

1. **Heredoc with mixed redirections and pipes:**

   ```bash
   cat <minishell.h <<HERE <missing <<DOC | echo oi
   ```

   This works, but the redirection order may not match standard shell behavior.

2. **Exit code and empty variables:**

   ```bash
   doesntexist      # correctly sets exit code to 127
   $EMPTY           # should reset exit code to 0, but doesn't
   echo $?
   ```

---

## 🔧 Technical Details

Coming soon: a deep dive into how parsing, forking, expansion, redirection, and execution work under the hood.

---

## 👥 Authors

Project by the team at **LinuxHeads** — built with ❤️ for the 42 Network.
