# RPG_RL
Playing the MMORPG using the Deep Reinforcement Learning. The Nethack and Diablo1 game will be used for environment.

# Reference
1. NetHack: https://github.com/facebookresearch/nle
2. NetHack Visualization: https://github.com/maciej-sypetkowski/autoascend
3. Diablo1 for Linux: https://github.com/diasurgical/devilutionX
4. Vizdoom: https://github.com/mwydmuch/ViZDoom
5. Simple SDL2 game: https://github.com/PolyMarsDev/Twini-Golf

# NetHack
Visualization tool is added to original NLE of Facebook code.

<img src="image/vis_1.png" width="1000">

You can play a game yourself by below command.

```
$ python3.8 -m nle.scripts.play --env NetHackChallenge-v0
```

# Simple SDL game
Move to [Twini-Golf folder](https://github.com/kimbring2/RPG_RL/tree/main/Twini-Golf) and build project using below command.

```
$ cmake .
$ make
```

Then, move to [lib_python folder](https://github.com/kimbring2/RPG_RL/tree/main/Twini-Golf/src/lib_python). You should check there is 'vizgolf.cpython-38-x86_64-linux-gnu.so' that was built. 

Open Jupyter Notebook for 'env_test.ipynb' or run Python for 'test_code.py'.
