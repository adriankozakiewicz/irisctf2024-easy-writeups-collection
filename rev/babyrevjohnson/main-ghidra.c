
undefined8 main(void)

{
  int iVar1;
  long in_FS_OFFSET;
  int local_84;
  int local_80;
  int local_7c;
  char local_78 [104];
  long local_10;
  
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  puts("Welcome to the Johnson\'s family!");
  puts(
      "You have gotten to know each person decently well, so let\'s see if you remember all of the f acts."
      );
  puts("(Remember that each of the members like different things from each other.)");
  local_84 = 0;
  while (local_84 < 4) {
    printf("Please choose %s\'s favorite color: ",*(undefined8 *)(names + (long)local_84 * 8));
    __isoc99_scanf(&DAT_0010217c,local_78);
    iVar1 = strcmp(local_78,colors);
    if (iVar1 == 0) {
      local_80 = 1;
LAB_00101449:
      if ((((local_80 == chosenColors) || (local_80 == DAT_00104094)) || (local_80 == DAT_00104098))
         || (local_80 == DAT_0010409c)) {
        puts("That option was already chosen!");
      }
      else {
        (&chosenColors)[local_84] = local_80;
        local_84 = local_84 + 1;
      }
    }
    else {
      iVar1 = strcmp(local_78,PTR_DAT_00104048);
      if (iVar1 == 0) {
        local_80 = 2;
        goto LAB_00101449;
      }
      iVar1 = strcmp(local_78,PTR_s_green_00104050);
      if (iVar1 == 0) {
        local_80 = 3;
        goto LAB_00101449;
      }
      iVar1 = strcmp(local_78,PTR_s_yellow_00104058);
      if (iVar1 == 0) {
        local_80 = 4;
        goto LAB_00101449;
      }
      puts("Invalid color!");
    }
  }
  local_84 = 0;
  do {
    while( true ) {
      if (3 < local_84) {
        check();
        if (local_10 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
          __stack_chk_fail();
        }
        return 0;
      }
      printf("Please choose %s\'s favorite food: ",*(undefined8 *)(names + (long)local_84 * 8));
      __isoc99_scanf(&DAT_0010217c,local_78);
      iVar1 = strcmp(local_78,foods);
      if (iVar1 != 0) break;
      local_7c = 1;
LAB_0010159c:
      if (((local_7c == chosenFoods) || (local_7c == DAT_001040a4)) ||
         ((local_7c == DAT_001040a8 || (local_7c == DAT_001040ac)))) {
        puts("That option was already chosen!");
      }
      else {
        (&chosenFoods)[local_84] = local_7c;
        local_84 = local_84 + 1;
      }
    }
    iVar1 = strcmp(local_78,PTR_s_pasta_00104068);
    if (iVar1 == 0) {
      local_7c = 2;
      goto LAB_0010159c;
    }
    iVar1 = strcmp(local_78,PTR_s_steak_00104070);
    if (iVar1 == 0) {
      local_7c = 3;
      goto LAB_0010159c;
    }
    iVar1 = strcmp(local_78,PTR_s_chicken_00104078);
    if (iVar1 == 0) {
      local_7c = 4;
      goto LAB_0010159c;
    }
    puts("Invalid food!");
  } while( true );
}
