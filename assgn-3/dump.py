# from functools import filter
import os

def dump_csv(file_name, out):
    outfile = open(out, 'w')
    outfile.write(
        ',page scan,active->inactive,active->laundry,inactive->active,inactive->free,inactive->laundry,laundry->active,laundry->free\r\n'
    )
    last = None
    i = 0
    infile = open(file_name, 'r')
    for line in infile:
        if line.find('last message') != -1:
            for _ in range(
                    int(line[(line.find('repeated') + 8):(len(line) - 6)])):
                outfile.write(last)
        else:
            last = str(i) + line[29:(len(line) - 1)].replace('| ', '').replace(
                ' ', ',') + '\r\n'
            outfile.write(last)
        i += 1
    infile.close()
    outfile.close()


raws = os.listdir('./raw')
for f in raws:
    dump_csv('./raw/' + f, './csvs/' + f + '.csv')

mod = open('./csvs/debug-mod-clear.log.csv', 'r')
org = open('./csvs/debug-org-clear.log.csv', 'r')

all_tables = [{
    'mod': [],
    'org': []
}, {
    'mod': [],
    'org': []
}, {
    'mod': [],
    'org': []
}, {
    'mod': [],
    'org': []
}, {
    'mod': [],
    'org': []
}, {
    'mod': [],
    'org': []
}]

index = 0
for line in mod:
    if index == 0:
        index += 1
        continue
    all_number = enumerate(line.split(','))
    for i, num in all_number:
        if i == 0 or i > 6:
            continue
        dic = all_tables[i - 1]
        dic['mod'].append(num)
    index += 1

index = 0
for line in org:
    if index == 0:
        index += 1
        continue
    all_number = enumerate(line.split(','))
    for i, num in all_number:
        if i == 0 or i > 6:
            continue
        dic = all_tables[i - 1]
        dic['org'].append(num)
    index += 1

index_mapping = {
    0: 'page-scan',
    1: 'active-inactive',
    2: 'active-laundry',
    3: 'inactive-active',
    4: 'inactive-free',
    5: 'inactive-laundry'
}

for index in range(len(all_tables)):
    table = all_tables[index]
    filename = './compare/' + index_mapping[index] + '.csv'
    out = open(filename, 'w')
    out.write(',mod,org\r\n')
    modl = table['mod']
    orgl = table['org']
    length = min(len(modl), len(orgl))
    for i in range(length):
        line = str(i) + ',' + modl[i] + ',' + orgl[i] + '\r\n'
        out.write(line)
    out.close()
        
mod.close()
org.close()