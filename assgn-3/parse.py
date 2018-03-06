# from functools import filter
import os

def dump_csv(file_name, out):
    outfile = open(out, 'w')
    outfile.write(',page scan,active->inactive,active->laundry,inactive->active,inactive->free,inactive->laundry,laundry->active,laundry->free\r\n')
    file = open(file_name, 'r')
    last = None
    i = 0
    for line in file:
        if line.find('last message') != -1:
            repeat_at = line.find('repeated')
            count = int(line[(repeat_at + 8):(len(line) - 6)])
            for _ in range(count):
                outfile.write(last)
                print(last)
        else:
            last = str(i) + line[29:(len(line) - 1)].replace('| ', '').replace(' ', ',') + '\r\n'
            outfile.write(last)
            print(last)
        i += 1

raws = os.listdir('./raw')
for f in raws:
    dump_csv('./raw/' + f, './csvs/' + f + '.csv')
    
# print(ls)