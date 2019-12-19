We create threads for students,serving tables and the chefs.

rob_fn():Thread function for hef.

biryani_ready():When the chef have created all the vessels of biryani,he'll invoke this function.

empty():Checks which container is empty.

ser_fn():Thread function for serving table.

ready_to_serve():Once the serving container is allocated,it invokes the function.

allocate():Allocates students to respective serving tables.

stud_fn():Thread function for student.

wait_for_slot():Student waiting for a slot at the serving table.

student_in_slot():Student allocated on serving table starts eating.﻿
