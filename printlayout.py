import os
import sys
import math
import matplotlib.pyplot as plt
import matplotlib.patches as patches

blocks = []
area = 0
ratio = 0
width = 0
height = 0

class Block:
    def __init__(self, b_id, b_x, b_y, b_width, b_height, b_rotate=False):
        self.b_id = b_id
        self.b_x = b_x
        self.b_y = b_y
        self.b_width = b_width
        self.b_height = b_height
        self.b_rotate = b_rotate

def parse_blocksize(f):
    for line in f:
        if line[0] == 'b':
            # b1 40 50
            block = line.split()
            b_id = block[0]
            b_width = block[1]
            b_height = block[2]
            blocks.append(Block(b_id, 0, 0, b_width, b_height))

def parse_blockcoordinate(f):
    for line in f:
        if line[0] == 'b':
            # b1 40 50 (R)
            block = line.split()
            b_id, b_x, b_y, *b_rotate = block
            for b in blocks:
                if b.b_id == b_id:
                    b.b_x = b_x
                    b.b_y = b_y
                    if 'R' in b_rotate:
                        b.b_width, b.b_height = b.b_height, b.b_width
        elif line[0] == 'A':
            global area 
            area = int(line.split()[2])
        elif line[0] == 'R':
            global ratio 
            ratio = float(line.split()[2])
    
    global width 
    width = math.sqrt(area * ratio)
    global height 
    height = area / width

def plot():
    fig, ax = plt.subplots()
    ax.set_xlim(-10, width + 10)
    ax.set_ylim(-10, height + 10)
    ax.set_xlabel('X Axis')
    ax.set_ylabel('Y Axis')
    ax.xaxis.grid(True, linestyle='--', linewidth=0.5, color='gray')
    ax.yaxis.grid(True, linestyle='--', linewidth=0.5, color='gray')
    for block in blocks:
        b_x, b_y, b_width, b_height = float(block.b_x), float(block.b_y), float(block.b_width), float(block.b_height)
        rect = patches.Rectangle((b_x, b_y), b_width, b_height, linewidth=1, edgecolor='black', facecolor='blue', alpha=0.5)
        ax.add_patch(rect)

        # label_0 = f'{block.b_id}'
        # ax.text(b_x + b_width/2, b_y + b_height/2, label_0, fontsize=12, ha='center', va='center', color='white')
        # label_0 = f'{block.b_id}\n{int(b_width)},{int(b_height)}'
        # ax.text(b_x + b_width/2, b_y + b_height/2, label_0, fontsize=12, ha='center', va='center', color='white')
        # label_1 = f'({int(b_x)},{int(b_y)})'
        # ax.text(b_x , b_y, label_1, fontsize=12, ha='left', va='bottom')
        # ax.scatter(b_x, b_y, marker='*', color='red', s=100, label='Left Bottom')

    fig.savefig(os.path.join(os.path.dirname(__file__), 'figures', 'plot.png'))

if __name__ == '__main__':

    if len(sys.argv) != 3:
        print("Usage: python printlayout.py input.in output.out")
        sys.exit(1)

    block_size = sys.argv[1]
    block_coordinate = sys.argv[2]

    with open(block_size, 'r') as f:
        parse_blocksize(f)

    with open(block_coordinate, 'r') as f:
        parse_blockcoordinate(f)
        
    plot()

    print("Layout Done!")
    sys.exit(0)

    