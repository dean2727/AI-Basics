// 5
int State::heuristic(State* goal) {
    int h = 0;

    vector<vector<char> > goalConfig = goal -> getConfig();
    int thisBlockConfigSize = blockConfig.size(), goalBlockConfigSize = goalConfig.size();
    int thisStackSize, goalStackSize;
    // maps each block to the stack they are on
    map<char, int> stackNums;

    // maps each block in the goal state to the block that should be under it
    map<char, char> underBlocks;
    for (int gbcs = 0; gbcs < goalBlockConfigSize; gbcs++) {  // gcbs = goal block config stack
        goalStackSize = goalConfig[gbcs].size();
        for (int gb = 0; gb < goalStackSize; gb++) {  // gb = goal block
            stackNums.insert(make_pair(goalConfig[gbcs][gb], gbcs));

            // no block under it, just have a placeholder of !
            if (gb == 0) {
                underBlocks.insert(make_pair(goalConfig[gbcs][gb], '!'));
            }
            else {
                underBlocks.insert(make_pair(goalConfig[gbcs][gb], goalConfig[gbcs][gb-1]));
            }
        }
    }

    char thisBlock, blockUnder;

    for (int tbcs = 0; tbcs < thisBlockConfigSize; tbcs++) {  // tbcs = this block config stack
        thisStackSize = blockConfig[tbcs].size();
        bool badUnder = false;
        for (int tb = 0; tb < thisStackSize; tb++) {  // tb = this block
            thisBlock = blockConfig[tbcs][tb];
            if (tb == 0) {
                blockUnder = '!';
            }
            else {
                blockUnder = blockConfig[tbcs][tb - 1];
            }

            if (blockUnder == '!' && stackNums[thisBlock] != tbcs) {
                h += 2;
                badUnder = true;
            }

            // if block under is not equal, add 2
            else if (blockUnder != underBlocks[thisBlock]) {
                h += 2;
                badUnder = true;
            }

            // even if the block under is fine, if we've seen a wrong under block in the pile, add 2
            else if (badUnder) h += 2;            
        }
    }

    return h;
}