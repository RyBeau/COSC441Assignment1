import statistics

# Global Constants
EXPERIMENT_1 = "Experiment1"
EXPERIMENT_2 = "Experiment2"
EXPERIMENT_3 = "Experiment3"
OVERHEAD_BITS = 100
CSV_HEADER = "Packet Length,Node Distance,Average Packet Loss Rate, Packet Loss StDev,Average Goodput, Goodput StDev\n"
DIRNAME = "C:/Users/Ryan/Desktop/MyProjects/Omnet/COSC441Assignment1/simulations/results/"

# Experiment 1 Constants
EXPERIMENT_1_DISTANCES = [i for i in range(10, 51)]
EXPERIMENT_1_PACKET_LENGTHS = [400, 900]
EXPERIMENT_1_FILENAME = "{}-{}-{}#{}.sca"
# Experiment 2 Constants
EXPERIMENT_2_DISTANCES = [35]
EXPERIMENT_2_PACKET_LENGTHS = [i for i in range(100, 1000, 100)]
EXPERIMENT_FILENAME = "{}-{}-#{}.sca"
# Experiment 3 Constants
EXPERIMENT_3_DISTANCES = [35]
EXPERIMENT_3_PACKET_LENGTHS = [i for i in range(100, 1000, 100)]


def get_mean_from_file(filename):
    file = open(DIRNAME + filename, "r")
    lines = file.readlines()
    mean_line = lines[30].split()
    if "mean" not in mean_line:
        raise AssertionError("Not the mean")
    file.close()
    return float(mean_line[-1])


def calculate_goodputs(packet_loss_averages, packet_length):
    goodputs = []
    for packet_loss_rate in packet_loss_averages:
        goodputs.append(((1 - packet_loss_rate) * packet_length) / (packet_length + OVERHEAD_BITS))
    return goodputs


def get_loss_averages(experiment, packet_length, node_distance):
    packet_loss_averages = []
    for repetition in range(0, 100):
        if experiment == EXPERIMENT_1:
            filename = EXPERIMENT_1_FILENAME.format(experiment, packet_length, node_distance, repetition)
        else:
            filename = EXPERIMENT_FILENAME.format(experiment, packet_length, repetition)
        packet_loss_averages.append(get_mean_from_file(filename))
    return packet_loss_averages


def average(numbers):
    return sum(numbers) / len(numbers)


def calculate_confidence_intervals():
    pass


def process_experiment(experiment, packet_lengths, node_distances):
    lines_to_write = [CSV_HEADER]
    for packet_length in packet_lengths:
        for node_distance in node_distances:
            packet_loss_averages = get_loss_averages(experiment, packet_length, node_distance)
            average_packet_loss = average(packet_loss_averages)
            packet_loss_std = statistics.stdev(packet_loss_averages, average_packet_loss)
            goodputs = calculate_goodputs(packet_loss_averages, packet_length)
            average_goodput = average(goodputs)
            goodput_std = statistics.stdev(goodputs, average_goodput)
            lines_to_write.append("{},{},{},{},{},{}\n".format(packet_length, node_distance, average_packet_loss,
                                                               packet_loss_std, average_goodput, goodput_std))
    file = open("{}ResultsTest.csv".format(experiment), "w")
    file.writelines(lines_to_write)
    file.close()
    print("{} processed".format(experiment))


def main():
    experiment_to_process = input("Which experiment to process? ")
    if experiment_to_process == "1":
        print("Processing experiment 1")
        process_experiment(EXPERIMENT_1, EXPERIMENT_1_PACKET_LENGTHS, EXPERIMENT_1_DISTANCES)
    elif experiment_to_process == "2":
        print("Processing experiment 2")
        process_experiment(EXPERIMENT_2, EXPERIMENT_2_PACKET_LENGTHS, EXPERIMENT_2_DISTANCES)
    elif experiment_to_process == "3":
        print("Processing experiment 3")
        process_experiment(EXPERIMENT_3, EXPERIMENT_3_PACKET_LENGTHS, EXPERIMENT_3_DISTANCES)
    else:
        print("Processing all experiments")
        process_experiment(EXPERIMENT_1, EXPERIMENT_1_PACKET_LENGTHS, EXPERIMENT_1_DISTANCES)
        process_experiment(EXPERIMENT_2, EXPERIMENT_2_PACKET_LENGTHS, EXPERIMENT_2_DISTANCES)
        process_experiment(EXPERIMENT_3, EXPERIMENT_3_PACKET_LENGTHS, EXPERIMENT_3_DISTANCES)
    print("Completed")


main()
