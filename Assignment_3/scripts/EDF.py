import heapq

# These constants are used to describe the input data format
# Specifying what each column is holding information about

TASK_NAME_COL = 0
DEADLINES_COL = 1

TASK_POWER_INFO_ROW = 0
TASK_TIMING_INFO_START_ROW = 1

WCET_AT_F1_COL = 2
WCET_AT_F2_COL = 3
WCET_AT_F3_COL = 4
WCET_AT_F4_COL = 5

IDLE_POWER_COL = 6

frequency = ['1188', '918', '648', '384']


def get_utilization(wcet, deadlines):
    u = 0
    for i in range(len(wcet)):
        u += wcet[i] / deadlines[i]  # here deadline=period
    return u


def find_edf_task(deadline):
    min_val = min(deadline)
    min_index = deadline.index(min_val)
    return min_index


def edf_scheduling(wcet, deadlines, max_time, idle_power, active_power, freq_value):
    output = []

    current_time = 0
    total_energy_consumed = 0
    total_idle_time = 0

    # list of upcoming task deadline
    task_next_deadlines = deadlines.copy()
    task_ready_list = [0, 0, 0, 0, 0]
    remaining_task = []
    while current_time < max_time:
        # find the earliest deadline task
        task_index = find_edf_task(task_next_deadlines)

        if current_time < task_ready_list[task_index]:

            start_time = current_time
            is_idle = True
            idle_time = task_ready_list[task_index] - current_time
            current_time = task_ready_list[task_index]
            total_idle_time += idle_time
            energy_consumed = idle_power * idle_time / 1000
            total_energy_consumed += energy_consumed
            output.append([start_time, 'W' + str(task_index), 'IDLE', idle_time, str(energy_consumed) + 'J'])

        else:
            # update the deadlines for next decision point
            task_next_deadlines[task_index] += deadlines[task_index]
            start_time = current_time
            current_time += wcet[task_index]
            task_ready_list[task_index] += deadlines[task_index]
            energy_consumed = active_power * wcet[task_index] / 1000
            total_energy_consumed += energy_consumed
            output.append([start_time, 'W' + str(task_index), freq_value, wcet[task_index], str(energy_consumed) + 'J'])

    return output, \
           ('Total Energy Consumed = ' + str(total_energy_consumed) + 'J'), \
           ('Total Idle Time = ' + str(total_idle_time / 10) + '%'), \
           ('Total execution time ' + str(1000 - total_idle_time) + 'sec')


def main():
    # open input file
    file = open("./input2.txt", "r")

    # read line from input file
    # Note that input file is space delimited
    parameters_matrix = []
    for line in file:
        line = line.split(" ")
        parameters_matrix.append(line)

    # convert string to numerical data
    for values in parameters_matrix:
        for i in range(1, len(values)):
            values[i] = int(values[i])

    # get number of task
    num_of_tasks = int(parameters_matrix[0][0])

    # get maximum hyper time
    max_hyper_time = parameters_matrix[0][1]

    # get idle power
    idle_power = parameters_matrix[0][IDLE_POWER_COL]

    # get active powers
    active_power_f1 = parameters_matrix[0][2]
    active_power_f4 = parameters_matrix[0][3]
    active_power_f3 = parameters_matrix[0][4]
    active_power_f4 = parameters_matrix[0][5]

    # get deadlines information from the parameters_matrix
    task_deadlines = []
    for row in range(TASK_TIMING_INFO_START_ROW, len(parameters_matrix)):
        task_deadlines.append(parameters_matrix[row][DEADLINES_COL])

    # get wcet information for cpu frequency = 1188

    task_wcet_list = [[0], [0], [0], [0]]

    task_wcet_f1 = []
    task_wcet_f2 = []
    task_wcet_f3 = []
    task_wcet_f4 = []

    for row in range(TASK_TIMING_INFO_START_ROW, len(parameters_matrix)):
        task_wcet_f1.append(parameters_matrix[row][WCET_AT_F1_COL])

    task_wcet_list[0] = task_wcet_f1

    for row in range(TASK_TIMING_INFO_START_ROW, len(parameters_matrix)):
        task_wcet_f2.append(parameters_matrix[row][WCET_AT_F2_COL])

    task_wcet_list[1] = task_wcet_f2

    for row in range(TASK_TIMING_INFO_START_ROW, len(parameters_matrix)):
        task_wcet_f3.append(parameters_matrix[row][WCET_AT_F3_COL])

    task_wcet_list[2] = task_wcet_f3

    for row in range(TASK_TIMING_INFO_START_ROW, len(parameters_matrix)):
        task_wcet_f4.append(parameters_matrix[row][WCET_AT_F4_COL])

    task_wcet_list[3] = task_wcet_f4

    # now that we have gathered all the information
    # lets check utilization for each frequency

    utilization = [0, 0, 0, 0]
    utilization[0] = get_utilization(task_wcet_list[0], task_deadlines)
    utilization[1] = get_utilization(task_wcet_list[1], task_deadlines)
    utilization[2] = get_utilization(task_wcet_list[2], task_deadlines)
    utilization[3] = get_utilization(task_wcet_list[3], task_deadlines)

    index = utilization.index(min(utilization))

    print(get_utilization(task_wcet_f1, task_deadlines))
    print(get_utilization(task_wcet_f2, task_deadlines))
    print(get_utilization(task_wcet_f3, task_deadlines))
    print(get_utilization(task_wcet_f4, task_deadlines))

    if get_utilization(task_wcet_list[index], task_deadlines) < 1.0:

        output, total_energy_consumed, total_idle_time, total_exe_time = edf_scheduling(task_wcet_list[index],
                                                                                        task_deadlines,
                                                                                        max_hyper_time,
                                                                                        idle_power,
                                                                                        active_power_f1,
                                                                                        frequency[index])

    file = open("output.txt", "w")
    for row in output:
        for i in range(len(row)):
            row[i] = str(row[i])
        file.write(" ".join(row) + '\n')

    file.write(total_energy_consumed + '\n')
    file.write(total_idle_time + '\n')
    file.write(total_exe_time)
    file.close()


if __name__ == "__main__":
    main()
