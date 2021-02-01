# copyright 2020, Thomas R. Ioerger, All rights reserved.

import sys,random

def error(s):
  print "error: "+s
  sys.exit(-1)
  
if len(sys.argv)!=4:
  error("usage: python blockgen.py <#stacks> <#blocks> <#steps>")

S = int(sys.argv[1])
B = int(sys.argv[2])
N = int(sys.argv[3])

if S<1 or S>100: error("number of stacks should be 1-100")
if B<1 or B>26: error("number of blocks should be 1-26")
if N<0 or N>1000000: error("number of steps should be 0-1000000")

BLOCKS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

class State:
  def __init__(self,S,B):
    self.S,self.B = S,B
    if self.S==-1: return
    self.stacks = []
    for i in range(S): self.stacks.append([])
    self.stacks[0] = range(B) # all blocks stacked in order on 1st stack by default
  def randomize_state(self):
    self.stacks = []
    for i in range(S): self.stacks.append([])
    for i in range(self.B):
      j = random.randrange(S)
      self.stacks[j].append(i)
  def copy(self):
    newstate = State(-1,-1)
    newstate.S,newstate.B = self.S,self.B
    newstate.stacks = []
    for i in range(self.S): 
      newstate.stacks.append([x for x in self.stacks[i]])
    return newstate
  def print_(self):
    for i in range(S):
      blocks = ""
      for j in range(len(self.stacks[i])): 
        blocks += BLOCKS[self.stacks[i][j]]
      print blocks
  # returns a new state (i.e. a copy that is modified)
  def random_move(self):
    newstate = self.copy()
    i,j = -1,-1
    # pick a source stack with at least 1 block in it
    while i==-1 or len(newstate.stacks[i])==0: i = random.randrange(self.S)
    # pick a different target stack
    while j==-1 or j==i: j = random.randrange(self.S)
    newstate.stacks[j].append(newstate.stacks[i][-1])
    newstate.stacks[i] = newstate.stacks[i][:-1]
    return newstate
      

init = State(S,B)
init.randomize_state()
state = init
for i in range(N): state = state.random_move()
goal = state

print "%s %s %s" % (S,B,N)
print ">>>>>>>>>>"
init.print_()
print ">>>>>>>>>>"
goal.print_()
print ">>>>>>>>>>"
