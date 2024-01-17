
void check(void)

{
  byte bVar1;
  bool bVar2;
  
  if ((DAT_001040a8 == 2) || (DAT_001040ac == 2)) {
    bVar2 = false;
  }
  else {
    bVar2 = true;
  }
  if ((chosenColors == 3) || (DAT_00104094 == 3)) {
    bVar1 = 0;
  }
  else {
    bVar1 = 1;
  }
  if (DAT_0010409c == 2 &&
      (DAT_00104098 != 4 &&
      (DAT_001040ac != 3 && (chosenFoods == 4 && (bool)((DAT_00104094 != 1 && bVar2) & bVar1))))) {
    puts("Correct!");
    system("cat flag.txt");
  }
  else {
    puts("Incorrect.");
  }
  return;
}
