

/**************************************
 * IIR filtar drugog reda
 *
 * input - ulazni odbirak
 * coefficients - koeficijenti [a0 a1 a2 b0 b1 b2]
 * z_x - memorija za ulazne odbirke (niz duzine 2)
 * z_y - memorija za izlazne odbirke (niz duzine 2)
 *
 * povratna vrednost - izlazni odbirak
 *
 *************************************/

double second_order_IIR(double input, double* coefficients, double* x_history, double* y_history) {
    double output = 0;

    output += coefficients[0] * input);        /* A0 * x(n)     */
    output += coefficients[1] * x_history[0]; /* A1 * x(n-1) */
    output += coefficients[2] * x_history[1]; /* A2 * x(n-2)   */
    output -= coefficients[4] * y_history[0]; /* B1 * y(n-1) */
    output -= coefficients[5] * y_history[1]; /* B2 * y(n-2)   */

    
    y_history[1] = y_history[0];    /* y(n-2) = y(n-1) */
    y_history[0] = output; /* y(n-1) = y(n)   */
    x_history [1] = x_history [0];  /* x(n-2) = x(n-1) */
    x_history [0] = input;          /* x(n-1) = x(n)   */

    return output;
}

/**************************************
 * IIR filtar N-tog reda
 *
 * input - ulazni odbirak
 * coefficients - koeficijenti [a0 a1 a2 b0 b1 b2] za svaku fazu
 * z_x - memorija za ulazne odbirke (niz duzine N_STAGE * 2)
 * z_y - memorija za izlazne odbirke (niz duzine N_STAGE * 2)
 * N_STAGE - broj faza (sekcija drugog reda)
 * povratna vrednost - izlazni odbirak
 *
 *************************************/

double Nth_order_IIR(double input, double coefficients[][6], double x_history[][2],
		double y_history[][2], unsigned int N_STAGES) {
	double output = 0;
	double temp;
	unsigned int stage;

    temp = input; /* Copy input to temp */

	for ( stage = 0 ; stage < N_STAGES ; stage++) /* for two stages */
	{
		output += coefficients[stage][0] * temp);               /* A0 * x(n)     */
		output += coefficients[stage][1] * x_history[stage][0]; /* A1 * x(n-1) */
		output += coefficients[stage][2] * x_history[stage][1];   /* A2 * x(n-2)   */
		output -= coefficients[stage][4] * y_history[stage][0]; /* B1 * y(n-1) */
		output -= coefficients[stage][5] * y_history[stage][1]; /* B2 * y(n-2)   */

		/* Shuffle values along one place for next time */
		y_history[stage][1] = y_history[stage][0];    /* y(n-2) = y(n-1) */
		y_history[stage][0] = output; /* y(n-1) = y(n)   */
		x_history[stage][1] = x_history[stage][0];  /* x(n-2) = x(n-1) */
		x_history[stage][0] = temp;          /* x(n-1) = x(n)   */

		/* temp is used as input next time through */
		temp = output;
	}
	/* End loop */

	return output;
}


