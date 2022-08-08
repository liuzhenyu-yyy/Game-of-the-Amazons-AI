# Game of the Amazons AI
 A simple DFS-based bot for the [Game of the Amazons](https://en.wikipedia.org/wiki/Game_of_the_Amazons), with optimized scoring function.
 To perform game, just run main.exe.

## Description
 This is my code for the Amazons Lab in "Introduction to Computation (A)"  Course of [Prof. Yafei Dai](https://cs.pku.edu.cn/info/1182/1515.htm) in the automn semaster of 2018. The Amazons Lab required us to develop a c/c++ program for the [Game of the Amazons](https://en.wikipedia.org/wiki/Game_of_the_Amazons) with following features:
 
 - Main menu for selection (new game, save game, start new game, quit);
 - Chess board and piece indicated by ASCII art;
 - Allowing player versus player and player versus computer;
 - Able to save current game and load saved games.
 
 The final score of Amazons Lab depends on the complexity，stability，interface of the software, and more importantly, **the performance of each AI**, as defined by the rankings in the ladder tournament at [botzone](https://www.botzone.org.cn/)
 
 ## Demonstrations:
 <img align="center" src="https://raw.githubusercontent.com/liuzhenyu-yyy/Game-of-the-Amazons-AI/main/assets/demo.png" />    

## Scoring function
 Scoring function defines the "fitness" of each piece move. Here I made profound optimizations in the scoring function. 
 
### Composition
The final score of each piece move is the linear combination of 5 evaluations:
- Territorial evaluation t1 and t2.
- Positional evaluation c1 and c2.
- Mobility evaluation m.
Detailed explanation of each evaluations could be find in the [reference](https://www.sciencedirect.com/science/article/pii/S0304397505005979).

### Weight of each evaluation.
In different stage, different evaluation values conribute differently to final devisions. So the weight of each evaluation should be a function of time. The weight function was optimized by AI competitions until convergence.

## Searching strategy
The software utilze depth-first search to identify the best movement at current situation, as achieved by recursive and recursion of several different chess moving functions. I only designed single-layed search and performs no pruning step during searching, which could be further improved.
