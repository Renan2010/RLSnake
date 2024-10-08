# RL Snake
**Photo of the game running**
![RLSnake](https://github.com/user-attachments/assets/f0a3ff20-db74-40cc-b842-12fba1c93bb4)
*Simple Snake Game in C++ with SFML*
## How to compile this game
**Clone this repository**
**using `git`**
```bash
~ $ git clone https://github.com/Renan2010/RLSnake.git
```
**Or using `github-cli`**
> [!Note]
**To use `github-cli` you need to allocate your github account, otherwise it won't work.**
```bash
~ $ gh repo clone Renan2010/RLSnake
```

**Enter the `RLSnake` directory**
```bash
~ $ cd RLSnake
```
**And run ninja to compile this game**
```bash
~/RLSnake $ ninja -j $(nproc)
```
**Execute `RLsnake`**
```bash
~/RLSnake $ ./RLSnake
```
*And enjoy :)*
# Controls

|   Button   |  Action     |
|------------|-------------|
| W,A,S,D    |   Move      |
# FAQ
**What is `$(nproc)`?**

*`$(nproc)` is the total number of cores, plus the threads on your machine/PC, the more cores, the faster the compilation*

**For exanple:**

*My PC is an Intel Core i5-2400 4C/4T*
```bash
~ $ nproc
Output: 4
```
*Other example AMD Ryzen 9 9950x 16C/32T* `<- best CPU for multi-core`
```bash
~ $ nproc
Output: 32
```
*in short, `$(nproc)` will take the cpu cores "automatically"*
