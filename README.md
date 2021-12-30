# TLB measurement

This C-Program measures the cost of accessing a TLB. The Code refers to the 19th Chapter of the book Operating Systems by Remzi H. Arpaci-Dusseau & Andrea C. Arpaci-Dusseau.
The basic idea behind the program is to measure the entered number of pages within an array-data structure and time those accesses.

## To view all flags enter:   
> ./tlb -h \
> -h, --help	Shows all Flags with description! \
> -n, --numpages	Defines number of pages \
> -l, --trials	Defines number of trials in order to get a reasonable mesaurement. \

## To run the program enter number of pages and number of trials: 
> ./tlb -n 64 -l 1000 \


To vizualize the hitrate of the TLB a python script has been added therefor a file 'Measurement.png' will be created when running the program. 


