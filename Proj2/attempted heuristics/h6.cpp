// 6
int State::heuristic(State* goal) {
    int h = 0;

    vector<vector<char> > goalConfig = goal -> getConfig();
    int thisBlockConfigSize = blockConfig.size(), goalBlockConfigSize = goalConfig.size();
    int thisStackSize, goalStackSize;

    // maps each block in the goal state to the block that should be under it
    map<char, char> underBlocks;
    // maps each block in the goal state to the block that should be over it
    map<char, char> overBlocks;
    for (int gbcs = 0; gbcs < goalBlockConfigSize; gbcs++) {  // gcbs = goal block config stack
        goalStackSize = goalConfig[gbcs].size();
        for (int gb = 0; gb < goalStackSize; gb++) {  // gb = goal block

            // no block under it, just have a placeholder of !
            if (gb == 0) {
                underBlocks.insert(make_pair(goalConfig[gbcs][gb], '!'));
            }
            else {
                underBlocks.insert(make_pair(goalConfig[gbcs][gb], goalConfig[gbcs][gb-1]));
            }

            // no block over it, since its the top of the stack, placeholder of !
            if (gb == goalStackSize - 1) {
                overBlocks.insert(make_pair(goalConfig[gbcs][gb], '!'));
            }
            else {
                overBlocks.insert(make_pair(goalConfig[gbcs][gb], goalConfig[gbcs][gb+1]));
            }
        }
    }

    char thisBlock, blockUnder, blockOver;

    for (int tbcs = 0; tbcs < thisBlockConfigSize; tbcs++) {  // tbcs = this block config stack
        thisStackSize = blockConfig[tbcs].size();
        for (int tb = 0; tb < thisStackSize; tb++) {  // tb = this block
            thisBlock = blockConfig[tbcs][tb];

            if (tb == 0) blockUnder = '!';
            else blockUnder = blockConfig[tbcs][tb - 1];

            if (tb == thisStackSize - 1) blockOver = '!';
            else blockOver = blockConfig[tbcs][tb + 1];

            // if block under is wrong or block over is wrong, add 2
            if (blockUnder != underBlocks[thisBlock] || blockOver != overBlocks[thisBlock]) {
                h += 2;
            }
        }
    }

    return h;
}