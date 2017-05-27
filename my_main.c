/*========== my_main.c ==========

  This is the only file you need to modify in order
  to get a working mdl project (for now).

  my_main.c will serve as the interpreter for mdl.
  When an mdl script goes through a lexer and parser, 
  the resulting operations will be in the array op[].

  Your job is to go through each entry in op and perform
  the required action from the list below:

  push: push a new origin matrix onto the origin stack
  pop: remove the top matrix on the origin stack

  move/scale/rotate: create a transformation matrix 
  based on the provided values, then 
  multiply the current top of the
  origins stack by it.

  box/sphere/torus: create a solid object based on the
  provided values. Store that in a 
  temporary matrix, multiply it by the
  current top of the origins stack, then
  call draw_polygons.

  line: create a line based on the provided values. Store 
  that in a temporary matrix, multiply it by the
  current top of the origins stack, then call draw_lines.

  save: call save_extension with the provided filename

  display: view the image live
  
  jdyrlandweaver
  =========================*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "parser.h"
#include "symtab.h"
#include "y.tab.h"

#include "matrix.h"
#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "stack.h"

/*======== void first_pass() ==========
  Inputs:   
  Returns: 

  Checks the op array for any animation commands
  (frames, basename, vary)
  
  Should set num_frames and basename if the frames 
  or basename commands are present

  If vary is found, but frames is not, the entire
  program should exit.

  If frames is found, but basename is not, set name
  to some default value, and print out a message
  with the name being used.

  jdyrlandweaver
  ====================*/
void first_pass() {
  //in order to use name and num_frames
  //they must be extern variables

  extern int num_frames;
  extern char name[128];
  num_frames = -1;
  int nameset;
  nameset = 0;
  int i;
  
  for (i=0;i<lastop;i++) {
    printf("%d: ",i);
    switch (op[i].opcode)
      {
      case FRAMES:
	if(num_frames == -1){
	  num_frames = op[i].op.frames.num_frames;
	}
	else{
	  printf("ERROR, FRAMES ALREADY SET");
	}
	break;
      case BASENAME:
	strcpy(name, op[i].op.basename.p->name);
	nameset = 1;
	break;
      case VARY:
	if(num_frames == -1){
	  printf("ERROR, FRAMES NOT SET. QUITTING PROGRAM");
	  exit(0);
	}
	break;
      }
  }
  if(num_frames != -1 && !nameset){
    strcpy(name, "default");
    printf("No basename set, setting to \"default\"");
  }
  printf("PASS 1 SUCCESFUL");
  fflush(stdout);
  return;
}

/*======== struct vary_node ** second_pass() ==========
  Inputs:   
  Returns: An array of vary_node linked lists

  In order to set the knobs for animation, we need to keep
  a seaprate value for each knob for each frame. We can do
  this by using an array of linked lists. Each array index
  will correspond to a frame (eg. knobs[0] would be the first
  frame, knobs[2] would be the 3rd frame and so on).

  Each index should contain a linked list of vary_nodes, each
  node contains a knob name, a value, and a pointer to the
  next node.

  Go through the opcode array, and when you find vary, go 
  from knobs[0] to knobs[frames-1] and add (or modify) the
  vary_node corresponding to the given knob with the
  appropirate value. 

  jdyrlandweaver
  ====================*/
struct vary_node ** second_pass() {
  /* printf("1\n"); */

  extern int num_frames;
  struct vary_node ** knobs;
  /* printf("2\n"); */
  knobs = (struct vary_node **)calloc(num_frames, sizeof(struct vary_node *));
  struct vary_node * tmpknob;
  /* tmp = (struct vary_node *)malloc(sizeof(struct vary_node)); */
  int i, j, num_nodes;
  num_nodes = 0;
  double val;
  val = 0;
  double start, end, min, max;

  int ctr;

  
  int nk [num_frames+1]; //num knobs
  for(ctr=0;ctr<num_frames+1;ctr++){
    nk[ctr] = 0;
    nk[48] = 0;
  }
	

  ctr = 0;
	  
  /* nk = (int *)malloc(num_frames); */

  
  /* printf("3\n"); */

  int k;
  for(k=0;k<num_frames;k++){
    knobs[k] = (struct vary_node *)malloc(sizeof(struct vary_node));
  }

  int numkno;
  numkno = 0;
  for (i=0;i<lastop;i++) {
    printf("4\n");
    fflush(stdout);
    switch (op[i].opcode)
      {
      case VARY:
	numkno++;
	break;
      }
  }
  

  for(i=0;i<num_frames;i++){
    tmpknob = knobs[i];
    for(k=0;k<numkno;k++){
      tmpknob->next = (struct vary_node *)malloc(sizeof(struct vary_node));
      tmpknob = tmpknob->next;
    }
  }
  
  for (i=0;i<lastop;i++) {
    printf("4\n");
    fflush(stdout);
    switch (op[i].opcode)
      {
      case VARY:
	for(j=op[i].op.vary.start_frame;j<=op[i].op.vary.end_frame;j++){
	  tmpknob = knobs[j];
	  printf("5\n");
	  fflush(stdout);
	  printf("Ctr: %d, NK[%d]: %d\n", ctr, j, nk[j]);
	  while(ctr < nk[j]){
	    tmpknob=tmpknob->next;
	    printf("5.5\n");
	    fflush(stdout);
	    ctr++;
	  }
	  /* tmpknob = (struct vary_node *)malloc(sizeof(struct vary_node)); */

	  /* memset(tmpknob->name, 0, 128);  */

	  /* tmpknob = (struct vary_node *)malloc(sizeof(struct vary_node)); */
	  printf("5.6\n");
	  fflush(stdout);
	  
	  /* if(knobs[0] != NULL){ */
	  /*   printf("5.6\n"); */
	  /*   fflush(stdout); */
	  /* } */
	  strcpy(tmpknob->name, op[i].op.vary.p->name);
	  printf("6\n");
	  printf("nameee %s\n", tmpknob->name);
	  fflush(stdout);
	
	  /* if(op[i].op.vary.start_frame >= j){ */
	  start = op[i].op.vary.start_frame;
	  end = op[i].op.vary.end_frame;
	  min = op[i].op.vary.start_val;
	  max = op[i].op.vary.end_val;
	  tmpknob->value = ((max - min) / (end - start)) * (j - op[i].op.vary.start_frame);
	  if(max < min){
	    printf("GOTTONUMBERRY\n\n");
	    tmpknob->value += min;
	  }
	  /* } */
	  printf("knob %s %d: %lf\n", tmpknob->name, j, tmpknob->value);
	  nk[j] += 1;
	  ctr = 0;
	}
	
	break;
      }
  }
  fflush(stdout);
  printf("SECOND PASS: %s, %lf\n\n\n", knobs[2]->name, knobs[2]->value);
  return knobs;
}


/*======== void print_knobs() ==========
  Inputs:   
  Returns: 

  Goes through symtab and display all the knobs and their
  currnt values

  jdyrlandweaver
  ====================*/
void print_knobs() {
  
  int i;

  printf( "ID\tNAME\t\tTYPE\t\tVALUE\n" );
  for ( i=0; i < lastsym; i++ ) {

    if ( symtab[i].type == SYM_VALUE ) {
      printf( "%d\t%s\t\t", i, symtab[i].name );

      printf( "SYM_VALUE\t");
      printf( "%6.2f\n", symtab[i].s.value);
    }
  }
}


/*======== void my_main() ==========
  Inputs: 
  Returns: 

  This is the main engine of the interpreter, it should
  handle most of the commadns in mdl.

  If frames is not present in the source (and therefore 
  num_frames is 1, then process_knobs should be called.

  If frames is present, the enitre op array must be
  applied frames time. At the end of each frame iteration
  save the current screen to a file named the
  provided basename plus a numeric string such that the
  files will be listed in order, then clear the screen and
  reset any other data structures that need it.

  Important note: you cannot just name your files in 
  regular sequence, like pic0, pic1, pic2, pic3... if that
  is done, then pic1, pic10, pic11... will come before pic2
  and so on. In order to keep things clear, add leading 0s
  to the numeric portion of the name. If you use sprintf, 
  you can use "%0xd" for this purpose. It will add at most
  x 0s in front of a number, if needed, so if used correctly,
  and x = 4, you would get numbers like 0001, 0002, 0011,
  0487

  jdyrlandweaver
  ====================*/
void my_main() {

  int i,f;
  struct matrix *tmp;
  struct stack *systems;
  screen t;
  color g;
  double step = 0.1;
  double theta;

  struct vary_node * tmpknoblist;
  tmpknoblist = (struct vary_node *)malloc(sizeof(struct vary_node));
  
  systems = new_stack();
  tmp = new_matrix(4, 1000);
  clear_screen( t );
  g.red = 0;
  g.green = 0;
  g.blue = 0;

  first_pass();
  struct vary_node ** knobs;
  if(num_frames > 1){
    knobs = second_pass();
    printf("\n\n\n======== POST SECOND PASS: %s, %lf========\n\n\n", knobs[2]->name, knobs[2]->value);
    /* print_knobs(); */
    for(f=0;f<num_frames;f++){
      systems = new_stack();
      printf("\n====tmpknoblist->name: %s====\n", tmpknoblist->name);
      for (i=0;i<lastop;i++) {

	printf("%d: ",i);
	fflush(stdout);
	switch (op[i].opcode)
	  {
	  case SPHERE:
	    printf("Sphere: %6.2f %6.2f %6.2f r=%6.2f",
		   op[i].op.sphere.d[0],op[i].op.sphere.d[1],
		   op[i].op.sphere.d[2],
		   op[i].op.sphere.r);
	    if (op[i].op.sphere.constants != NULL)
	      {
		printf("\tconstants: %s",op[i].op.sphere.constants->name);
	      }
	    if (op[i].op.sphere.cs != NULL)
	      {
		printf("\tcs: %s",op[i].op.sphere.cs->name);
	      }
	    add_sphere(tmp, op[i].op.sphere.d[0],
		       op[i].op.sphere.d[1],
		       op[i].op.sphere.d[2],
		       op[i].op.sphere.r, step);
	    /* print_matrix(tmp); */
	    /* print_matrix(peek(systems)); */
	    matrix_mult( peek(systems), tmp );
	    draw_polygons(tmp, t, g);
	    tmp->lastcol = 0;
	    break;
	  case TORUS:
	    printf("Torus: %6.2f %6.2f %6.2f r0=%6.2f r1=%6.2f",
		   op[i].op.torus.d[0],op[i].op.torus.d[1],
		   op[i].op.torus.d[2],
		   op[i].op.torus.r0,op[i].op.torus.r1);
	    if (op[i].op.torus.constants != NULL)
	      {
		printf("\tconstants: %s",op[i].op.torus.constants->name);
	      }
	    if (op[i].op.torus.cs != NULL)
	      {
		printf("\tcs: %s",op[i].op.torus.cs->name);
	      }
	    add_torus(tmp,
		      op[i].op.torus.d[0],
		      op[i].op.torus.d[1],
		      op[i].op.torus.d[2],
		      op[i].op.torus.r0,op[i].op.torus.r1, step);
	    matrix_mult( peek(systems), tmp );
	    draw_polygons(tmp, t, g);
	    tmp->lastcol = 0;	  
	    break;
	  case BOX:
	    printf("Box: d0: %6.2f %6.2f %6.2f d1: %6.2f %6.2f %6.2f",
		   op[i].op.box.d0[0],op[i].op.box.d0[1],
		   op[i].op.box.d0[2],
		   op[i].op.box.d1[0],op[i].op.box.d1[1],
		   op[i].op.box.d1[2]);
	    if (op[i].op.box.constants != NULL)
	      {
		printf("\tconstants: %s",op[i].op.box.constants->name);
	      }
	    if (op[i].op.box.cs != NULL)
	      {
		printf("\tcs: %s",op[i].op.box.cs->name);
	      }
	    add_box(tmp,
		    op[i].op.box.d0[0],op[i].op.box.d0[1],
		    op[i].op.box.d0[2],
		    op[i].op.box.d1[0],op[i].op.box.d1[1],
		    op[i].op.box.d1[2]);
	    matrix_mult( peek(systems), tmp );
	    draw_polygons(tmp, t, g);
	    tmp->lastcol = 0;
	    break;
	  case LINE:
	    printf("Line: from: %6.2f %6.2f %6.2f to: %6.2f %6.2f %6.2f",
		   op[i].op.line.p0[0],op[i].op.line.p0[1],
		   op[i].op.line.p0[1],
		   op[i].op.line.p1[0],op[i].op.line.p1[1],
		   op[i].op.line.p1[1]);
	    if (op[i].op.line.constants != NULL)
	      {
		printf("\n\tConstants: %s",op[i].op.line.constants->name);
	      }
	    if (op[i].op.line.cs0 != NULL)
	      {
		printf("\n\tCS0: %s",op[i].op.line.cs0->name);
	      }
	    if (op[i].op.line.cs1 != NULL)
	      {
		printf("\n\tCS1: %s",op[i].op.line.cs1->name);
	      }
	    break;
	  case MOVE:
	    tmpknoblist = knobs[f];
	    printf("Move: %6.2f %6.2f %6.2f",
		   op[i].op.move.d[0],op[i].op.move.d[1],
		   op[i].op.move.d[2]);
	    if (op[i].op.move.p != NULL){
	      printf("\tknob: %s",op[i].op.move.p->name);
	      while(tmpknoblist != NULL){
		if(strcmp(tmpknoblist->name, op[i].op.move.p->name) == 0){
		  printf("Vary move\n");
		  tmp = make_translate( op[i].op.move.d[0] * tmpknoblist->value,
				    op[i].op.move.d[1] * tmpknoblist->value,
				    op[i].op.move.d[2] * tmpknoblist->value);
		  break;
		}
		else{
		  tmpknoblist = tmpknoblist->next;
		}
	      }

	    }
	    else{
	      tmp = make_translate( op[i].op.move.d[0],
				    op[i].op.move.d[1],
				    op[i].op.move.d[2]);
	    }
	    matrix_mult(peek(systems), tmp);
	    copy_matrix(tmp, peek(systems));
	    /* matrix_mult(tmp, peek(systems)); */
	    tmp->lastcol = 0;
	    break;
	  case SCALE:
	    tmpknoblist = knobs[f];
	    printf("Scale: %6.2f %6.2f %6.2f",
		   op[i].op.scale.d[0],op[i].op.scale.d[1],
		   op[i].op.scale.d[2]);
	    if (op[i].op.scale.p != NULL){
	      printf("\tknob: %s",op[i].op.scale.p->name);
	      while(tmpknoblist != NULL){
		printf("Varyuuuu\n");
		if(strcmp(tmpknoblist->name, op[i].op.scale.p->name) == 0){
		  printf("Vary scale\n\n %lf %lf \n\n", op[i].op.scale.d[0], tmpknoblist->value);
		  tmp = make_scale( op[i].op.scale.d[0] * tmpknoblist->value,
				    op[i].op.scale.d[1] * tmpknoblist->value,
				    op[i].op.scale.d[2] * tmpknoblist->value);
		  /* print_matrix(tmp); */
		  break;
		}
		else{
		  printf("No Vary scale\n");
		  tmpknoblist = tmpknoblist->next;
		}
	      }
	    }
	    else{
	      printf("ELSA\n");
	      tmp = make_scale( op[i].op.scale.d[0],
				op[i].op.scale.d[1],
				op[i].op.scale.d[2]);
	    }
	    /* print_matrix(tmp); */
	    /* print_matrix(peek(systems)); */
	    matrix_mult(peek(systems), tmp);
	    /* print_matrix(tmp); */
	    /* print_matrix(peek(systems)); */
	    copy_matrix(tmp, systems->data[systems->top]);
	    /* print_matrix(tmp); */
	    /* print_matrix(peek(systems)); */
	    /* matrix_mult(tmp, systems->data[systems->top]); */
	    /* matrix_mult(tmp, peek(systems)); */
	    /* systems->data[systems->top] = tmp; */
	    tmp->lastcol = 0;
	    break;
	  case ROTATE:
	    tmpknoblist = knobs[f];
	    printf("\n====tmpknoblist->name: %s====\n", tmpknoblist->name);
	    printf("floooooooof\n");
	    /* printf("\n====op[i].op.rotate.p->name: %s====\n", op[i].op.rotate.p->name); */
	    printf("Rotate: axis: %6.2f degrees: %6.2f",
		   op[i].op.rotate.axis,
		   op[i].op.rotate.degrees);
	    if (op[i].op.rotate.p != NULL) {	
	      printf("\tknob: %s",op[i].op.rotate.p->name);
	      while(tmpknoblist != NULL){
		printf("Inloopooo\n");
		/* printf("\n====tmpknoblist->name: %s====\n", tmpknoblist->name); */
		/* printf("\n====op[i].op.rotate.p->name: %s====\n", op[i].op.rotate.p->name); */
		if(strcmp(tmpknoblist->name, op[i].op.rotate.p->name) == 0){
		  theta =  op[i].op.rotate.degrees * (M_PI / 180) * tmpknoblist->value;
		  printf("theeeeeeet %lf\n", theta);
		  fflush(stdout);
		  break;
		}
		else{
		  tmpknoblist = tmpknoblist->next;
		}
	      }
	    }//if there is a knob
	    else{
	      printf("theeeeeeetaaaaaaaaaaaaaa\n");
	      theta =  op[i].op.rotate.degrees * (M_PI / 180);
	      printf("theeeeeeet %lf\n", theta);
	    }//if there isn't a knob
	    if (op[i].op.rotate.axis == 0 )
	      tmp = make_rotX( theta );
	    else if (op[i].op.rotate.axis == 1 )
	      tmp = make_rotY( theta );
	    else
	      tmp = make_rotZ( theta );
	    /* print_matrix(tmp); */
	    printf("\n========theta: %lf========\n", theta);
	    
	    matrix_mult(peek(systems), tmp);
	    /* matrix_mult(tmp, peek(systems)); */
	    /* copy_matrix(tmp, systems->data[systems->top]); */
	    copy_matrix(tmp, peek(systems));
	    /* matrix_mult(tmp, systems->data[systems->top]); */
	    /* print_matrix(systems->data[systems->top]); */
	    /* fflush(stdout); */
	    /* printf("flooooo %lf\n", systems->data[systems->top]->m[0][0]); */
	    /* matrix_mult(tmp, peek(systems)); */
	    tmp->lastcol = 0;
	    break;
	  case PUSH:
	    printf("Push");
	    push(systems);
	    break;
	  case POP:
	    printf("Pop");
	    pop(systems);
	    break;
	  case SAVE:
	    printf("Save: %s",op[i].op.save.p->name);
	    save_extension(t, op[i].op.save.p->name);
	    break;
	  case DISPLAY:
	    printf("Display");
	    display(t);
	    break;
	  }
	printf("\n");
      }
      char fnum[3];
      sprintf(fnum, "%03d", f);
      char path[150];
      int cl;
      for(cl=0;cl<strlen(path);cl++){
	path[cl] = 0;
      }
      strcat(path, "anim/");
      strcat(path, name);
      strcat(path, fnum);
      save_extension(t, path);
      memset(path,0,sizeof(path));
      clear_screen(t);
      printf("Loop %d done\n", i);
    }
    printf("Got to end of if\n");
    printf("LASTOP: %d\n", lastop);
  }
  else{
    for (i=0;i<lastop;i++) {

      printf("%d: ",i);
      switch (op[i].opcode)
	{
	case SPHERE:
	  printf("Sphere: %6.2f %6.2f %6.2f r=%6.2f",
		 op[i].op.sphere.d[0],op[i].op.sphere.d[1],
		 op[i].op.sphere.d[2],
		 op[i].op.sphere.r);
	  if (op[i].op.sphere.constants != NULL)
	    {
	      printf("\tconstants: %s",op[i].op.sphere.constants->name);
	    }
	  if (op[i].op.sphere.cs != NULL)
	    {
	      printf("\tcs: %s",op[i].op.sphere.cs->name);
	    }
	  add_sphere(tmp, op[i].op.sphere.d[0],
		     op[i].op.sphere.d[1],
		     op[i].op.sphere.d[2],
		     op[i].op.sphere.r, step);
	  matrix_mult( peek(systems), tmp );
	  draw_polygons(tmp, t, g);
	  tmp->lastcol = 0;
	  break;
	case TORUS:
	  printf("Torus: %6.2f %6.2f %6.2f r0=%6.2f r1=%6.2f",
		 op[i].op.torus.d[0],op[i].op.torus.d[1],
		 op[i].op.torus.d[2],
		 op[i].op.torus.r0,op[i].op.torus.r1);
	  if (op[i].op.torus.constants != NULL)
	    {
	      printf("\tconstants: %s",op[i].op.torus.constants->name);
	    }
	  if (op[i].op.torus.cs != NULL)
	    {
	      printf("\tcs: %s",op[i].op.torus.cs->name);
	    }
	  add_torus(tmp,
		    op[i].op.torus.d[0],
		    op[i].op.torus.d[1],
		    op[i].op.torus.d[2],
		    op[i].op.torus.r0,op[i].op.torus.r1, step);
	  matrix_mult( peek(systems), tmp );
	  draw_polygons(tmp, t, g);
	  tmp->lastcol = 0;	  
	  break;
	case BOX:
	  printf("Box: d0: %6.2f %6.2f %6.2f d1: %6.2f %6.2f %6.2f",
		 op[i].op.box.d0[0],op[i].op.box.d0[1],
		 op[i].op.box.d0[2],
		 op[i].op.box.d1[0],op[i].op.box.d1[1],
		 op[i].op.box.d1[2]);
	  if (op[i].op.box.constants != NULL)
	    {
	      printf("\tconstants: %s",op[i].op.box.constants->name);
	    }
	  if (op[i].op.box.cs != NULL)
	    {
	      printf("\tcs: %s",op[i].op.box.cs->name);
	    }
	  add_box(tmp,
		  op[i].op.box.d0[0],op[i].op.box.d0[1],
		  op[i].op.box.d0[2],
		  op[i].op.box.d1[0],op[i].op.box.d1[1],
		  op[i].op.box.d1[2]);
	  matrix_mult( peek(systems), tmp );
	  draw_polygons(tmp, t, g);
	  tmp->lastcol = 0;
	  break;
	case LINE:
	  printf("Line: from: %6.2f %6.2f %6.2f to: %6.2f %6.2f %6.2f",
		 op[i].op.line.p0[0],op[i].op.line.p0[1],
		 op[i].op.line.p0[1],
		 op[i].op.line.p1[0],op[i].op.line.p1[1],
		 op[i].op.line.p1[1]);
	  if (op[i].op.line.constants != NULL)
	    {
	      printf("\n\tConstants: %s",op[i].op.line.constants->name);
	    }
	  if (op[i].op.line.cs0 != NULL)
	    {
	      printf("\n\tCS0: %s",op[i].op.line.cs0->name);
	    }
	  if (op[i].op.line.cs1 != NULL)
	    {
	      printf("\n\tCS1: %s",op[i].op.line.cs1->name);
	    }
	  break;
	case MOVE:
	  printf("Move: %6.2f %6.2f %6.2f",
		 op[i].op.move.d[0],op[i].op.move.d[1],
		 op[i].op.move.d[2]);
	  if (op[i].op.move.p != NULL)
	    {
	      printf("\tknob: %s",op[i].op.move.p->name);
	    }
	  tmp = make_translate( op[i].op.move.d[0],
				op[i].op.move.d[1],
				op[i].op.move.d[2]);
	  matrix_mult(peek(systems), tmp);
	  copy_matrix(tmp, peek(systems));
	  tmp->lastcol = 0;
	  break;
	case SCALE:
	  printf("Scale: %6.2f %6.2f %6.2f",
		 op[i].op.scale.d[0],op[i].op.scale.d[1],
		 op[i].op.scale.d[2]);
	  if (op[i].op.scale.p != NULL)
	    {
	      printf("\tknob: %s",op[i].op.scale.p->name);
	    }
	  tmp = make_scale( op[i].op.scale.d[0],
			    op[i].op.scale.d[1],
			    op[i].op.scale.d[2]);
	  matrix_mult(peek(systems), tmp);
	  copy_matrix(tmp, peek(systems));
	  tmp->lastcol = 0;
	  break;
	case ROTATE:
	  printf("Rotate: axis: %6.2f degrees: %6.2f",
		 op[i].op.rotate.axis,
		 op[i].op.rotate.degrees);
	  if (op[i].op.rotate.p != NULL)
	    {
	      printf("\tknob: %s",op[i].op.rotate.p->name);
	    }
	  theta =  op[i].op.rotate.degrees * (M_PI / 180);
	  if (op[i].op.rotate.axis == 0 )
	    tmp = make_rotX( theta );
	  else if (op[i].op.rotate.axis == 1 )
	    tmp = make_rotY( theta );
	  else
	    tmp = make_rotZ( theta );
	  
	  matrix_mult(peek(systems), tmp);
	  copy_matrix(tmp, peek(systems));
	  tmp->lastcol = 0;
	  break;
	case PUSH:
	  printf("Push");
	  push(systems);
	  break;
	case POP:
	  printf("Pop");
	  pop(systems);
	  break;
	case SAVE:
	  printf("Save: %s",op[i].op.save.p->name);
	  save_extension(t, op[i].op.save.p->name);
	  break;
	case DISPLAY:
	  printf("Display");
	  display(t);
	  break;
	}
      printf("\n");
    }
    printf("Got to end of else\n");
  }
  printf("Got to end of mymain\n");
  fflush(stdout);
  exit(0);
}
