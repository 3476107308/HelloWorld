# VSCode C++ Operation Practice

## Goal

Use VSCode to finish the first C++ pointer practice:

- Edit `main.cpp`
- Build the current file
- Run the generated `.exe`
- Set breakpoints and debug with `F5`
- Watch pointer expressions such as `p`, `*p`, and `&a`

## Step 1: Open This Folder

Open `D:\AAA study\week01_cpp_memory` in VSCode.

Recommended way:

```powershell
cd "D:\AAA study\week01_cpp_memory"
code .
```

## Step 2: Install Extensions

Open Extensions with `Ctrl+Shift+X`, then install:

- `C/C++` by Microsoft
- `CMake Tools`
- `Code Runner` optional

For this week, use VSCode's build/debug tasks first. Do not rely on Code Runner for debugging.

## Step 3: Build

Open `main.cpp`, then press:

```text
Ctrl+Shift+B
```

Choose `build active C++ file` if VSCode asks.

Expected result:

- VSCode compiles `main.cpp`
- A `main.exe` file appears in the same folder

## Step 4: Run In Terminal

In VSCode terminal:

```powershell
.\main.exe
```

You should see values like:

```text
a = 10
&a = 000000...
p = 000000...
*p = 10
```

Key idea:

- `&a` is the address of variable `a`
- `p` stores the same address
- `*p` reads the value at that address

## Step 5: Debug

In `main.cpp`:

1. Click the left side of the line `int* p = &a;` to set a breakpoint.
2. Press `F5`.
3. Use `F10` to step over line by line.
4. Open `Run and Debug`.
5. In `Watch`, add:

```text
a
&a
p
*p
```

Observe when `p` gets a real address and when `*p` becomes readable.

## Step 6: Practice Changes

After the first run succeeds, change `main.cpp` yourself:

1. Change `a` from `10` to another number.
2. Add `*p = 99;` after `int* p = &a;`.
3. Print `a` again and explain why `a` changed.

Suggested code:

```cpp
*p = 99;
cout << "after *p = 99, a = " << a << endl;
```

## Checkpoint

You are done with the VSCode operation part when you can:

- Build with `Ctrl+Shift+B`
- Run with `.\main.exe`
- Debug with `F5`
- Step with `F10`
- Explain `p`, `&p`, `*p`, and `&a`
