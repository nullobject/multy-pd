#include "multy.h"

enum DSP {
  PERFORM,
  OBJECT,
  INPUT1_VECTOR,
  INPUT2_VECTOR,
  OUTPUT_VECTOR,
  VECTOR_SIZE,
  NEXT
};

void multy_dsp(t_multy *x, t_signal **sp, short *count) {
  /* Attach the object to the DSP chain */
  dsp_add(multy_perform, NEXT - 1, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec,
          sp[0]->s_n);

  /* Print message to Max window */
  post("multy~ • Executing 32-bit perform routine");
}

t_int *multy_perform(t_int *w) {
  /* Copy the object pointer */
  // t_multy *x = (t_multy *)w[OBJECT];

  /* Copy signal pointers */
  t_float *in1 = (t_float *)w[INPUT1_VECTOR];
  t_float *in2 = (t_float *)w[INPUT2_VECTOR];
  t_float *out = (t_float *)w[OUTPUT_VECTOR];

  /* Copy the signal vector size */
  t_int n = w[VECTOR_SIZE];

  /* Perform the DSP loop */
  while (n--) {
    *out++ = *in1++ * *in2++;
  }

  /* Return the next address in the DSP chain */
  return w + NEXT;
}

void *multy_new(void) {
  /* Instantiate a new object */
  t_multy *x = (t_multy *)pd_new(multy_class);

  /* Create signal inlets */
  inlet_new(&x->obj, &x->obj.ob_pd, gensym("signal"), gensym("signal"));

  /* Create signal outlets */
  outlet_new(&x->obj, gensym("signal"));

  /* Print message to Max window */
  post("multy~ • Object was created");

  /* Return a pointer to the new object */
  return (void *)x;
}

void multy_free(t_multy *x) {
  // Nothing to free

  /* Print message to Max window */
  post("multy~ • Memory was freed");
}

void multy_tilde_setup(void) {
  /* Initialize the class */
  multy_class = class_new(gensym("multy~"), (t_newmethod)multy_new,
                          (t_method)multy_free, sizeof(t_multy), 0, 0);

  /* Specify signal input, with automatic float to signal conversion */
  CLASS_MAINSIGNALIN(multy_class, t_multy, x_f);

  /* Bind the DSP method, which is called when the DACs are turned on */
  class_addmethod(multy_class, (t_method)multy_dsp, gensym("dsp"), 0);

  /* Print message to Max window */
  post("multy~ • External was loaded");
}
