# Assignment1 Folder Guide

All about Assignment1 files is in the `assign1` folder.

---

## Algorithm Implement Code

1. **convention_algorithm.cc** : all contemporary algorithms (6) are here  
2. **contemporary_algorithm.cc** : all contemporary algorithms (6) are here  

---

## Experiment Code

1. **experiment.cc** : experiment for 1K~100K  
2. **experiment_1M.cc** : experiment for 1M  
3. **experiment_library.cc** : experiment for library.cc (for printing # of rebalances)  
4. **run_experiment.sh** : automatize all experiments of each algorithm  

---

## Result

1. **result(ms,B)** : result folder. It uses `mallinfo2()` (measuring heap memory, unit: B) for memory usage measurement. Other elements are same with other results folders.  
2. **result(ms,KB)** : result folder. It uses `ru_maxrss` method (measuring entire memory, unit: KB) for memory usage measurement. Other elements are same with other results folders.  
3. **result(1M_dataset)** : result folder for only 1M dataset (using `mallinfo2()` for memory usage measurement)  

---

> Other folders/files can be ignored.
