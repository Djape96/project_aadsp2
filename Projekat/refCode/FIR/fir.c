
double fir_circular(double input, double *coeffs, double *history, unsigned int n_coeff, unsigned int *p_state)
{
	int i;
    unsigned int state;
    double ret_val;

    state = *p_state;               /* copy the filter's state to a local */


	/* store input at the beginning of the delay line */
    history[state] = input;
	if (++state >= n_coeff)
	{         /* incr state and check for wrap */
		state = 0;
	}

	/* calc FIR and shift data */
	ret_val = 0;
	for (i = n_coeff - 1; i >= 0; i--)
	{
		ret_val += coeffs[i] * history[state];
		if (++state >= n_coeff) /* incr state and check for wrap */
		{
			state = 0;
		}
	}

    *p_state = state;               /* return new state to caller */

    return ret_val;
}
