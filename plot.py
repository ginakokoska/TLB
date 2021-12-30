#plot.py
#Autor: Gina Kokoska
#Datum: 14.11.21


import os
import sys
import matplotlib.pyplot as plt

argc = len(sys.argv)
argv = sys.argv

arrX = [2,4,16,32,64,128,265,512,1024]
plt.axis([0,1024,0,10])

fig, ax = plt.subplots()
ax.set_xscale('symlog', basex=2)

plt.xlabel('Number of Pages')
plt.ylabel('Time per Access(ns)')

arrTimes = []
arrPages = []
if argc > 1:
    cmd = './' + argv[1] + ' -n '
    for i in range(0,9):
        if os.path.isfile(argv[1]):
            ##print('Python Script: ', cmd + str(i))
            os.system(cmd + str(arrX[i]) + ' -l ' + str(argv[2]))
            datei = open('time.txt', 'r')
            ##print(datei.readline());
            arrTimes.append(int(datei.readline()))
            arrPages.append(arrX[i])
            datei.close()
        else:
            print('File doesnt exist!')
            exit(0)
else:
    print('To less arguments!')
    exit(0)




print(arrTimes)
print(arrPages)

plt.grid(True)
plt.plot(arrPages, arrTimes)
plt.savefig("Measure.png")
