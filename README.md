# Wordle in C++

Ένα console-based παιχνίδι τύπου Wordle γραμμένο σε C++.

## Χαρακτηριστικά
- Υποστήριξη χρωμάτων (Green/Yellow/Gray) μέσω Windows Console
- Προσθήκη/Διαγραφή λέξεων από αρχείο (`words.txt`)
- Προβολή στατιστικών παικτών (νίκες, ποσοστά, streaks)
- Μενού επιλογών: Παιχνίδι, Διαχείριση λέξεων, Στατιστικά
- Πλήρως playable σε Windows terminal

## Τεχνολογίες
- Γλώσσα: C++
- Compiler: GCC (με υποστήριξη C++11)
- Βιβλιοθήκες: `<iostream>`, `<vector>`, `<fstream>`, `<random>`, `<windows.h>`

## Τρόπος εκτέλεσης

### Dev-C++
1. Άνοιξε το αρχείο `main.cpp` ή `WORDLE.cpp`.
2. Πήγαινε:  
   `Tools → Compiler Options → Settings → Code Generation → Language standard`  
   και επίλεξε `-std=c++11`.
3. Κάνε Compile & Run.
