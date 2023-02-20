import statistics

d = {
    "BAT_MON":[],
    "BUT_MON":[],
    "CAM_MON":[],
    "CMD_MON":[],
    "CUR_MON":[],
    "FLT_MON":[],
    "IMU_MON":[],
    "RPT_MON":[],
    "RES_MON":[],
    "TMP_MON":[],
    "BRN_CON":[],
    "CAM_CON":[],
    "RCK_CON":[],
    "TMP_CON":[],
    "CYCLE" :[]
}

with open('output.log', 'r') as file:
    lines = file.readlines()
    for line in lines:
        if "_CON" in line or "_MON" in line or "CYCLE" in line:
            data = line.strip().split(':')
            d[data[0]].append(int(data[1]))

l = list(d.items())

for a in l:
    print(a[0])
    print( "Mean: " + "{:.2f}".format(statistics.fmean(a[1])) )
    print( "Max: " + "{:.2f}".format(max(a[1])) )
    print( "Std Dev: " + "{:.2f}".format(statistics.stdev(a[1])) )