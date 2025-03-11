import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pandas as pd
import sys
from pathlib import Path

def generate_path(home_folder=str(Path.home()), subfolder='/data/', basename='output', extension='txt'):
    """Generates the file path for saving the output"""
    output_folder = home_folder + subfolder
    filename = basename + '.' + extension
    output_path = output_folder + filename
    return output_path

def configure_animation(frame_count=125, fps=25):
    """Configures the animation settings"""
    run_time = frame_count / fps
    interval = max([int(1000 / fps), 1])
    time_step = interval / 1000
    times = np.linspace(0., run_time, int(frame_count))
    return times, interval, fps

def configure_rope(length=50, end_point=True, step_size=1, start=0, num_points=50):
    """Configures the rope's initial state for the animation"""
    points = num_points
    x_positions = np.linspace(start=start, stop=length, num=points, endpoint=end_point)
    y_positions = np.zeros(points)
    return x_positions, y_positions

def configure_plot(x_positions, y_positions):
    """Configures the plot for the animation"""
    fig, ax1 = plt.subplots(1, 1)
    ax1.set_ylim(-1.1, 1.1)
    rope, = ax1.plot(x_positions, y_positions, "o", markersize=5, color="green", label="points on string")
    ax1.legend(handles=[rope], bbox_to_anchor=(0, 0), loc='upper left')
    return fig, rope

def animate(time, data, rope):
    """Animation step function"""
    x_positions, y_positions = extract_position(data, i=time)
    rope.set_data(x_positions, y_positions)
    return rope,

def extract_position(data, i=0, other=2):
    """Extracts the positions of the points from the data"""
    row = data.iloc[i]
    y_positions = np.array(row[other:]).flatten()
    x_positions = np.arange(len(row) - other)
    return x_positions, y_positions

def get_data(filename, other=2):
    """Reads the data from a CSV file"""
    try:
        data = pd.read_csv(filename, skipinitialspace=True)
    except FileNotFoundError:
        print(f"The file {filename} does not exist. Please check the path.")
        exit(-1)

    num_positions = len(data.columns) - other
    num_times = len(data)
    return data, num_positions, num_times

def get_file_name(extension="txt"):
    """Get the filename from command-line arguments"""
    try:
        filename = sys.argv[1]
    except IndexError:
        print(f"You must specify a file to plot. Correct Usage: {sys.argv[0]} [FILENAME].{extension}")
        exit(-1)

    if (filename.split('.')[-1]) != extension:
        print(f"The file {filename} does not appear to be a {extension} file.")
        exit(-1)

    return filename

def main():
    """Main function to execute the program"""
    # Parsing command-line arguments
    if len(sys.argv) != 6:
        print("Usage: python script.py <csv_file> <cycles> <samples_per_cycle> <points> <output_file>")
        exit(-1)

    # Assign the arguments
    input_file = sys.argv[1]
    cycles = int(sys.argv[2])  # Number of cycles for the simulation
    samples_per_cycle = int(sys.argv[3])  # Number of samples per cycle
    points = int(sys.argv[4])  # Number of points along the string
    output_file = sys.argv[5]  # Output file name for the .gif

    # Get data from the CSV
    data, num_positions, num_times = get_data(input_file)

    # Configure animation settings
    times, interval, fps = configure_animation(frame_count=num_times)

    # Configure the rope (initial state of the string)
    x_positions, y_positions = configure_rope(length=50, num_points=points)

    # Set up the plot for the animation
    fig, rope = configure_plot(x_positions, y_positions)

    # Initialize animation
    ani = animation.FuncAnimation(fig, animate, num_times, interval=interval, blit=True, fargs=(data, rope))

    # Save the animation as a .gif file
    ani.save(output_file, writer="pillow", fps=fps)

if __name__ == "__main__":
    main()




