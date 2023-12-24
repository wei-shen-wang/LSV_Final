module c17(N7, N6, N3, N2, N1, key_0, key_1, key_2, N22_key, N23_key);
	input N7, N6, N3, N2, N1;
	input key_0, key_1, key_2;
	output N22_key, N23_key;
	wire N1, N2, N3, N6, N7, N10, N16, N11, N19, N22, N23;
	wire N23_key, N22_key, N1_key;
	wire key_0, key_1, key_2;
	assign N10 = ~( N1_key & N3 );
	assign N16 = ~( N2 & N11 );
	assign N11 = ~( N3 & N6 );
	assign N19 = ~( N11 & N7 );
	assign N22 = ~( N10 & N16 );
	assign N23 = ~( N16 & N19 );
	assign N23_key = ~( N23 ^ key_0 );
	assign N22_key = ~( N22 ^ key_1 );
	assign N1_key = ~( N1 ^ key_2 );
endmodule
