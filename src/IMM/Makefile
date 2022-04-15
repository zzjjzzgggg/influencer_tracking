all: imm_continuous imm_discrete

imm_continuous: *.cpp *.h
	g++ -DCONTINUOUS imm.cpp -Wall -std=c++0x -O3 sfmt/SFMT.c  -o imm_continuous

imm_discrete: *.cpp *.h
	g++ -DDISCRETE imm.cpp -Wall -std=c++0x -O3 sfmt/SFMT.c  -o imm_discrete

imm: *.cpp *.h
	g++ imm.cpp -Wall -std=c++0x -O3 sfmt/SFMT.c  -o imm
O0:
	g++ imm.cpp -Wall -std=c++0x -O0 sfmt/SFMT.c  -o imm
O1:
	g++ imm.cpp -Wall -std=c++0x -O1 sfmt/SFMT.c  -o imm
O2:
	g++ imm.cpp -Wall -std=c++0x -O2 sfmt/SFMT.c  -o imm
profile:
	g++ imm.cpp -Wall -std=c++0x -O2 sfmt/SFMT.c  -pg -o imm.profile

cont_profile:
	g++ -DCONTINUOUS imm.cpp -Wall -std=c++0x -O3 sfmt/SFMT.c  -o imm_continuous -pg -o imm_continuous.profile
