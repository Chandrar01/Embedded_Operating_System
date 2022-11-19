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


def find_task_priority(period):
    priority = []
    temp_list = period.copy()
    for i in range(0, len(period)):
        min_val = min(temp_list)
        min_index = temp_list.index(min_val)
        priority.append(period.index(min_val))
        temp_list.remove(min_val)

    return priority


def find_frequency_priority(f_energy):
    priority = []
    temp_list = f_energy.copy()
    for i in range(0, len(f_energy)):
        min_val = min(temp_list)
        min_index = temp_list.index(min_val)
        priority.append(f_energy.index(min_val))
        temp_list.remove(min_val)

    return priority


def rm_scheduling(wcet, deadlines, max_time, idle_power, active_power, freq_value):
    output = []

    current_time = 0
    total_energy_consumed = 0
    total_idle_time = 0

    # list of upcoming task deadline
    task_period = deadlines.copy()
    task_deadline = deadlines.copy()
    task_ready_list = [0, 0, 0, 0, 0]
    remaining_task = []
    priority_task = 0

    while current_time < max_time:
        if priority_task > 4:
            priority_task = 0

        task_priority = find_task_priority(task_period)
        current_task = task_priority[priority_task]
        if current_time < task_ready_list[current_task]:

            start_time = current_time
            is_idle = True
            idle_time = task_ready_list[current_task] - current_time
            current_time = task_ready_list[current_task]
            total_idle_time += idle_time
            energy_consumed = idle_power * idle_time / 1000
            total_energy_consumed += energy_consumed
            output.append([start_time,
                           'W' + str(current_task + 1),
                           'IDLE',
                           idle_time,
                           str(energy_consumed) + 'J',
                           task_deadline[current_task]])

        else:
            start_time = current_time
            current_time += wcet[current_task]
            task_ready_list[current_task] += deadlines[current_task]
            energy_consumed = active_power * wcet[current_task] / 1000
            total_energy_consumed += energy_consumed
            output.append([start_time,
                           'W' + str(current_task + 1),
                           freq_value, wcet[current_task],
                           str(energy_consumed) + 'J',
                          task_deadline[current_task]])
            priority_task += 1
            task_deadline[current_task] += deadlines[current_task]

    return output, \
           ('Total Energy Consumed = ' + str(total_energy_consumed) + 'J'), \
           ('Total Idle Time = ' + str(total_idle_time / 10) + '%'), \
           ('Total execution time ' + str(1000 - total_idle_time) + 'sec')


def main():
    # open input file
    file = open("./input2.txt", "r")
    output = ""
    total_energy_consumed = ""
    total_idle_time = ""
    total_exe_time = ""
    is_feasible = False
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

    active_power_f = [0, 0, 0, 0]
    # get active powers
    active_power_f[0] = parameters_matrix[0][2]
    active_power_f[1] = parameters_matrix[0][3]
    active_power_f[2] = parameters_matrix[0][4]
    active_power_f[3] = parameters_matrix[0][5]

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

    # find minimum utilization index
    index = utilization.index(min(utilization))

    print(get_utilization(task_wcet_f1, task_deadlines))
    print(get_utilization(task_wcet_f2, task_deadlines))
    print(get_utilization(task_wcet_f3, task_deadlines))
    print(get_utilization(task_wcet_f4, task_deadlines))

    # find which tasks are suitable for reduced frequency
    # for task 1 /w1
    ok_to_reduce = [0, 0, 0, 0]
    w1_f_energy = [0, 0, 0, 0]
    w2_f_energy = [0, 0, 0, 0]
    w3_f_energy = [0, 0, 0, 0]
    w4_f_energy = [0, 0, 0, 0]
    w5_f_energy = [0, 0, 0, 0]

    for i in range(0, 4):
        w1_f_energy[i] = (task_wcet_list[i][0] * active_power_f[i])
        w2_f_energy[i] = (task_wcet_list[i][1] * active_power_f[i])
        w3_f_energy[i] = (task_wcet_list[i][2] * active_power_f[i])
        w4_f_energy[i] = (task_wcet_list[i][3] * active_power_f[i])
        w5_f_energy[i] = (task_wcet_list[i][4] * active_power_f[i])

    f_index_priority = find_frequency_priority(w1_f_energy)

    if get_utilization(task_wcet_list[f_index_priority[0]], task_deadlines) < 1.0:
        is_feasible = True
        output, total_energy_consumed, total_idle_time, total_exe_time = rm_scheduling(
            task_wcet_list[f_index_priority[0]],
            task_deadlines,
            max_hyper_time,
            idle_power,
            active_power_f[f_index_priority[0]],
            frequency[f_index_priority[0]])

    elif get_utilization(task_wcet_list[f_index_priority[1]], task_deadlines) < 1.0:
        is_feasible = True
        output, total_energy_consumed, total_idle_time, total_exe_time = rm_scheduling(
            task_wcet_list[f_index_priority[1]],
            task_deadlines,
            max_hyper_time,
            idle_power,
            active_power_f[f_index_priority[1]],
            frequency[f_index_priority[1]])

    elif get_utilization(task_wcet_list[f_index_priority[2]], task_deadlines) < 1.0:
        is_feasible = True
        output, total_energy_consumed, total_idle_time, total_exe_time = rm_scheduling(
            task_wcet_list[f_index_priority[2]],
            task_deadlines,
            max_hyper_time,
            idle_power,
            active_power_f[f_index_priority[2]],
            frequency[f_index_priority[2]])

    elif get_utilization(task_wcet_list[f_index_priority[3]], task_deadlines) < 1.0:
        is_feasible = True
        output, total_energy_consumed, total_idle_time, total_exe_time = rm_scheduling(
            task_wcet_list[f_index_priority[3]],
            task_deadlines,
            max_hyper_time,
            idle_power,
            active_power_f[f_index_priority[3]],
            frequency[f_index_priority[3]])

    file = open("EE_RM_output.txt", "w")
    if is_feasible:
        for row in output:
            for i in range(len(row)):
                row[i] = str(row[i])
            file.write(" ".join(row) + '\n')

        file.write(total_energy_consumed + '\n')
        file.write(total_idle_time + '\n')
        file.write(total_exe_time)
        file.close()
    else:
        file.write("No Feasible Schedule is Found")
        file.close()


if __name__ == "__main__":
    main()
