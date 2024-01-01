# LSV Final - Synthesis for Security

## Files Structure

    R12943102_R12943103.zip
        R12943102_R12943103/
        |-- LSV_Final/
        |    |-- abc/
        |    |-- abc_script/
        |    |-- benchmarks/
        |    |-- bin/
        |    |-- src/
        |    |-- makefile
        |    |-- README.md
        |    |-- runall_abc.sh
        |    |-- runall_insertKey.sh
        |    `-- runall_orig.sh
        |-- Reference_Paper/
        |    |-- Security_analysis_of_logic_obfuscation.pdf
        |    |-- Secuirty_Analysis_of_IC_Camouflaging.pdf
        |    `-- Ending_Piracy_of_ICs.pdf
        |-- LSV_final_report.pdf
        |-- LSV_final_presentation.pdf
        `-- README.md

- `LSV_Final`
    - `src`: Our source codes for implementation of Synthesis for Security.
        - Type `make` to compile the program. The executable named `obfuscate` will be generated under the `bin` folder.
            ```
            make
            ``` 
        - Type `make clean` to remove the `obfuscate` under `bin`.
            ```
            make clean
            ```
    - `bin`: Binary - store the `obfuscate` binary executable mentioned above.
        - Type 
            ```
            bin/obfuscate benchmarks/c17.v -insertKeyGate
            ```
            to generate an obfuscated netlist `c17_out.v`.
            ```
            bin/obfuscate benchmarks/c17.v
            ```
            to generate an original netlist `c17_orig.v` but in the form that abc can read.
    - `abc`: This folder is the same folder cloned from berkeley abc [5]. Type `make` **inside** the `abc` folder to compile and generate an `abc` executable.
    - `benchmarks`: This folder contains the some of the benchmarks cloned from [4].
    - `runall_orig.sh`: Type `./runall_orig.sh` to generate all original netlists but in the form that `abc` can read.
    - `runall_insertKey.sh`: Type `./runall_insertKey.sh` to generate all obfuscated netlists (also can  be read by `abc`).
    - `abc_script`: Contain scripts read the verilog file and transfrom it into an aig and print out its number and level(depth) of and-gates.
        - example `c17_out.src`:
            ```
            read_verilog c17_out.v
            strash
            print_stats
            ```
        - Type `./abc/abc -f c17_out.src` to execute the script and see the statistics of transformed aig.
    - `runall_abc.sh`: Type `runall_abc.sh` to runall the scripts under the folder `abc_script`. The terminal standard output is redirected to benchmark`.v.log` under the folder `abc_script`.
    - `README.md`: The same `README.md` under the folder `r12943102_r12943103`.
- `Reference_Paper`:
    - `Security_analysis_of_logic_obfuscation.pdf`: J. Rajendran, Y. Pino, O. Sinanoglu and R. Karri, "Security analysis of logic obfuscation," DAC Design Automation Conference 2012, San Francisco, CA, USA, 2012, pp. 83-89 [1]
    - `Secuirty_Analysis_of_IC_Camouflaging.pdf`: Jeyavijayan Rajendran, Michael Sam, Ozgur Sinanoglu, and Ramesh Karri. 2013. Security analysis of integrated circuit camouflaging. In Proceedings of the 2013 ACM SIGSAC conference on Computer & communications security (CCS '13). Association for Computing Machinery, New York, NY, USA, 709–720. [2]
    - `Ending_Piracy_of_ICs.pdf`: Jarrod A. Roy, Farinaz Koushanfar, and Igor L. Markov. 2008. EPIC: ending piracy of integrated circuits. In Proceedings of the conference on Design, automation and test in Europe (DATE '08). Association for Computing Machinery, New York, NY, USA, 1069–1074. [3]
- `LSV_final_report.pdf`: Our final report in the pdf format.
- `LSV_final_presentation.pdf`: Our final presentation slides.
- `README.md`: Describe what are included in the zip file.
    
## Reference

[1] J. Rajendran, Y. Pino, O. Sinanoglu and R. Karri, "Security analysis of logic obfuscation," DAC Design Automation Conference 2012, San Francisco, CA, USA, 2012, pp. 83-89

[2] Jeyavijayan Rajendran, Michael Sam, Ozgur Sinanoglu, and Ramesh Karri. 2013. Security analysis of integrated circuit camouflaging. In Proceedings of the 2013 ACM SIGSAC conference on Computer & communications security (CCS '13). Association for Computing Machinery, New York, NY, USA, 709–720.

[3] Jarrod A. Roy, Farinaz Koushanfar, and Igor L. Markov. 2008. EPIC: ending piracy of integrated circuits. In Proceedings of the conference on Design, automation and test in Europe (DATE '08). Association for Computing Machinery, New York, NY, USA, 1069–1074.

[4] https://github.com/jpsety/verilog_benchmark_circuits

[5] https://github.com/berkeley-abc/abc