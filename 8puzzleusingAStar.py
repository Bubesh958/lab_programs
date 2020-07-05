
import random
import math

_goal_state = [[3,2,6],
               [4,5,1],
               [7,8,0]]

def index(item, seq):
    """Helper function that returns -1 for non-found index value of a seq"""
    if item in seq:
        return seq.index(item)
    else:
        return -1

class EightPuzzle:

    def __init__(self):
        # heuristic value
        self._hval = 0
        # search depth of current instance
        self._depth = 0
        # parent node in search path
        self._parent = None
        self.adj_matrix = []
        for i in range(3):
            self.adj_matrix.append(_goal_state[i][:])   # duplicating goal_state
        
    def __eq__(self, other):                     # overriding ==
        if self.__class__ != other.__class__:
            return False
        else:
            return self.adj_matrix == other.adj_matrix

    def __str__(self):                          # overriding ""
        res = ''
        #print("String called")
        for row in range(3):
            res += ' '.join(map(str, self.adj_matrix[row]))
            res += '\r\n'
        return res

    def _clone(self):
        p = EightPuzzle()
        for i in range(3):
            p.adj_matrix[i] = self.adj_matrix[i][:]
        return p

    def _get_legal_moves(self):
        """Returns list of tuples with which the free space may
        be swapped"""
        # get row and column of the empty piece
        row, col = self.find(0)
        free = []
        
        # find which pieces can move there
        if row > 0:
            free.append((row - 1, col))
        if col > 0:
            free.append((row, col - 1))
        if row < 2:
            free.append((row + 1, col))
        if col < 2:
            free.append((row, col + 1))

        return free

    def _generate_moves(self):
        free = self._get_legal_moves()
        zero = self.find(0)
        
        def swap_and_clone(a, b):
            p = self._clone()
            p.swap(a,b)
            p._depth = self._depth + 1
            p._parent = self
            return(p)

        return map(lambda pair: swap_and_clone(zero, pair), free)

    def _generate_solution_path(self, path):
        if self._parent == None:
            return path
        else:
            path.append(self)
            return self._parent._generate_solution_path(path)

    def solve(self, h):
        """Performs A* search for goal state.
        h(puzzle) - heuristic function, returns an integer
        """
        def is_solved(puzzle):
            return puzzle.adj_matrix == _goal_state
        openl = [self]
        closedl = []
        move_count = 0
        while len(openl) > 0:
            x = openl.pop(0)
            move_count += 1
            if (is_solved(x)):
                if len(closedl) > 0:
                    return x._generate_solution_path([]), move_count
                else:
                    return [x]

            succ = x._generate_moves()
            #print(list(succ)[0].adj_matrix)
            idx_open = idx_closed = -1
            for move in succ:
                # have we already seen this node?
                idx_open = index(move, openl)
                idx_closed = index(move, closedl)
                hval = h(move)
                fval = hval + move._depth

                if idx_closed == -1 and idx_open == -1: #we haven't created this node
                    move._hval = hval
                    openl.append(move)
                elif idx_open > -1:      # similar node already existing in openl
                    copy = openl[idx_open]
                    if fval < copy._hval + copy._depth:
                        # copy move's values over existing
                        copy._hval = hval
                        copy._parent = move._parent
                        copy._depth = move._depth
                elif idx_closed > -1:  #similar node already existing in closedl(alredy visited)
                    copy = closedl[idx_closed]
                    if fval < copy._hval + copy._depth:
                        move._hval = hval
                        closedl.remove(copy)
                        openl.append(move)

            closedl.append(x)
            openl = sorted(openl, key=lambda p: p._hval + p._depth)

        # if finished state not found, return failure
        return [], 0

    def shuffle(self, step_count):
        for i in range(step_count):
            row, col = self.find(0)     # getting the position of blank tile
            free = self._get_legal_moves()       # getting the legal moves  blank tile
            target = random.choice(free)   #choosing one move(random) from legal moves
            self.swap((row, col), target)            
            row, col = target

    def find(self, value):
        """returns the row, col coordinates of the specified value
           in the graph"""
        if value < 0 or value > 8:
            raise Exception("value out of range")

        for row in range(3):
            for col in range(3):
                if self.adj_matrix[row][col] == value:
                    return row, col
    
    def peek(self, row, col):
        """returns the value at the specified row and column"""
        return self.adj_matrix[row][col]

    def poke(self, row, col, value):
        """sets the value at the specified row and column"""
        self.adj_matrix[row][col] = value

    def swap(self, pos_a, pos_b):
        """swaps values at the specified coordinates"""
        temp = self.peek(*pos_a)
        self.poke(pos_a[0], pos_a[1], self.peek(*pos_b))
        self.poke(pos_b[0], pos_b[1], temp)


def find_target_col_goal_state(val):
    for row in _goal_state:
        i=0;
        for col in row:
            if val == col:
                return i
            i = i+1

            
def find_target_row_goal_state(val):
    i=0
    for row in _goal_state:
        for col in row:
            if val == col:
                return i
        i = i+1
        
        
def heur(puzzle, item_total_calc, total_calc):
    t = 0
    for row in range(3):
        for col in range(3):
            val = puzzle.peek(row, col)
            target_col = find_target_col_goal_state(val)
            target_row = find_target_row_goal_state(val)
            if target_row < 0: 
                target_row = 2
            
            t += item_total_calc(row, target_row, col, target_col)
    return total_calc(t)



    
""" Number of moves to reach the goal state"""
def h_manhattan(puzzle):
    return heur(puzzle,
                lambda r, tr, c, tc: abs(tr - r) + abs(tc - c),
                lambda t : t)
""" Number of tiles misplaced from the goal state"""
def h_hamming(puzzle):
    return heur(puzzle,
               lambda r, tr, c, tc: 1 if _goal_state[tr][tc] == puzzle.adj_matrix[r][c] else 0,
               lambda t : t)
""" Number of tiles misplaced in each row and column from the goal state"""
def h_simple(puzzle):
    return heur(puzzle,
                lambda r, tr, c, tc: (abs(tr - r) + abs(tc - c))**2,
                lambda t: math.sqrt(t))


def main():
    p = EightPuzzle() #creating instance of the class
    p.shuffle(20)
    path, count = p.solve(h_simple)
    path.reverse()
    for i in path: 
        print(i)
    print ("Solved with manhattan exploring", count, "states")
    path, count = p.solve(h_manhattan)
    print ("Solved with hamming exploring", count, "states")
    path, count = p.solve(h_hamming)
    print ("Solved with simple exploring", count, "states")
    path, count = p.solve(h_simple)

if __name__ == "__main__":
    main()