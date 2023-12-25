module c17(N1, N2, N3, N6, N7, N22, N23);
	input N1, N2, N3, N6, N7;
	output N22, N23;
	wire N1, N2, N3, N6, N7, N10, N16, N11, N19, N22, N23;
	assign N10 = ~( N1 & N3 );
	assign N16 = ~( N2 & N11 );
	assign N11 = ~( N3 & N6 );
	assign N19 = ~( N11 & N7 );
	assign N22 = ~( N10 & N16 );
	assign N23 = ~( N16 & N19 );
endmodule
