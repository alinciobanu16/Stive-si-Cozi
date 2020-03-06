/*Ciobanu Alin-Matei 315CB*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "biblioteca.h"


int main(int argc, char *argv[]) {
	if (argc != 3) {  // invalid number of arguments
    	fprintf(stderr, "Usage: %s operations_string input_file output_file\n",
    	 argv[0]);
    return -1;
	}

	FILE *in_file = fopen(argv[1], "r");
	if (in_file == NULL) {
		fprintf(stderr, "File \"%s\" not found\n", argv[1]);
	return -2;
	}

	FILE *out_file = fopen(argv[2], "w");
	if (out_file == NULL) {
    	fprintf(stderr, "Could not open or create file \"%s\"\n", argv[2]);
    	return -2;
	}

	//FILE *in_file = fopen("input", "r");
	//FILE *out_file = fopen("output", "w");

	int T, nrp = 1, cnt = 0, PIDD, mem_size, exec_time, prioritate,
	 size, timp, t_finish = 0, total_size = 0, ok, ok2;
	char operatie[100], str[100];
	TProces p, *running = NULL;
	TLG LP = NULL, LM = NULL;
	void *ca, *cf;
	ca = InitQ(sizeof(TProces)); /*coada de waiting*/
	if (!ca) 
		return 0;

	cf = InitQ(sizeof(TProces)); /*coada de finished*/
	if (!cf) {
		DistrQ(ca);
		return 0;
	}

	fscanf(in_file, "%d", &T);
	while (fscanf(in_file, "%s", operatie) == 1) {
		if (strcmp(operatie, "add") == 0) {
			fscanf(in_file, "%d %d %d", &mem_size, &exec_time, &prioritate);
			cnt++;
			p.s = InitS(sizeof(4));
			if (!p.s) {
				DistrQ(ca);
				DistrQ(cf);
				return 0;
			}
			p.mem_size = mem_size;
			p.mem_ramasa = mem_size;
			p.exec_time = exec_time;
			p.prioritate = prioritate;
			p.timp_ramas = exec_time;
			if (T <= p.exec_time) {
				p.cuanta_ramasa = T;
			} else {
				p.cuanta_ramasa = p.exec_time;
			}
			GenereazaListaPid(&LP, &nrp);
			AdaugaPid(&p, LP);
			adaugaInListaMem(&LM, &p, &total_size, &ok);
			if (ok == 0) { /*inseamna ca procesul nu a avut loc in memorie*/
				total_size -= p.mem_size; /*il scad din memoria totala
				fiindca in functia de adaugaInLista l-am adunat*/
				defragmentare(LM); /*realizez o defragmentare a sistemului
				si incerc din nou adaugarea procesului*/
				adaugaInListaMem(&LM, &p, &total_size, &ok2); 
				if (ok2 == 0) {
					/*daca nu reuseste trece la urmatoarea operatie*/
					fprintf(out_file, "Cannot reserve memory for PID %d.\n", p.PID);
					total_size -= p.mem_size;
					eliberarePid(&LP, p.PID);
					free(p.s);
					continue;
				}
			}
			/*daca se ajunge aici inseamna ca procesul a fost adaugat
			si se afiseaza mesajul de succes*/
			afis_add(LM, p.PID, out_file, afisM);
			if (cnt == 1 || running == NULL) {
				/*daca sunt la primul proces adaugat il trec in stare running*/
				running = malloc(sizeof(TProces));
				if (!running) {
					free(p.s);
					DistrQ(ca);
					DistrQ(cf);
					return 1;
				}
				running->mem_size = p.mem_size;
				running->mem_ramasa = p.mem_ramasa;
				running->s = p.s;
				running->exec_time = p.exec_time;
				running->prioritate = p.prioritate;
				running->timp_ramas = p.timp_ramas;
				running->cuanta_ramasa = p.cuanta_ramasa;
				running->PID = p.PID;
			} else {
				/*altfel il adaug in coada de asteptare*/
				ca = add(ca, p);
		}
	}

		if (strcmp(operatie, "get") == 0) {
			fscanf(in_file, "%d", &PIDD);
			get(ca, cf, running, PIDD, out_file);
		}

		if (strcmp(operatie, "push") == 0) {
			fscanf(in_file, "%d %d", &PIDD, &size);
			push_s(ca, running, PIDD, size, out_file);
		}

		if (strcmp(operatie, "pop") == 0) {
			fscanf(in_file, "%d", &PIDD);
			pop_s(ca, running, PIDD, out_file);
		}

		if (strcmp(operatie, "print") == 0) {
			fscanf(in_file, "%s", str);
			if (strcmp(str, "stack") == 0) {
				fscanf(in_file, "%d", &PIDD);
				print_stack(ca, running, PIDD, out_file);
			}

			if (strcmp(str, "waiting") == 0) {
				print_waiting(ca, out_file);
			}

			if (strcmp(str, "finished") == 0) {
				print_finished(cf, out_file);
			}
		}

		if (strcmp(operatie, "run") == 0 ) {
			fscanf(in_file, "%d", &timp);

			while (timp > 0 && running != NULL) {
				if (VidaQ(ca) == 1) {
					/*daca nu am niciun proces in coada de asteptare*/
					if (timp >= running->timp_ramas) {
						/*ruleaza procesul din running pana cand il termina
						si il sterge din memorie si il adauga in finished*/
						eliberarePid(&LP, running->PID);
						stergeDinListaMem(&LM, running);
						total_size -= running->mem_size;
						IntrQ(cf, running);
						free(running);
						running = NULL;
					} else {
						/*daca timpul de run este mai mic decat timpul ramas
						al procesului actualizez timpii procesului si il las
						in running*/
						running->timp_ramas -= timp;
						timp -= running->cuanta_ramasa;
						running->cuanta_ramasa = T - (timp - T * (timp / T)); 
						if (running->timp_ramas < running->cuanta_ramasa) {
							running->cuanta_ramasa = running->timp_ramas;
						} 
					}
					timp = 0;

				} else if (VidaQ(ca) == 0) {
					/*daca am mai multe procese trebuie extras un nou proces
					din coada de asteptare dupa terminarea celui curent*/
					if (timp < running->cuanta_ramasa) {
						/*procesul curent nu s-a terminat
						il las in running si actualizez timpii*/
						running->timp_ramas -= timp;
						running->cuanta_ramasa -= timp;
						timp = 0;
					} else {
						/*extrag un nou proces*/
						running->timp_ramas -= running->cuanta_ramasa;
						timp -= running->cuanta_ramasa;
						if (T < running->timp_ramas) {
							running->cuanta_ramasa = T;
						} else {
							running->cuanta_ramasa = running->timp_ramas;
						}
						TProces new_running; // *new_running
						ExtrQ(ca, &new_running); //ExtrQ(cw, )

						if (running->timp_ramas > 0) {
							/*daca nu i s-a terminat timpul il pun in waiting*/
							ca = add(ca, *running);
						
						} else {
							/*altfel il adaug in finish si il sterg din
							memorie*/
							eliberarePid(&LP, running->PID);
							stergeDinListaMem(&LM, running);
							total_size -= running->mem_size;
							IntrQ(cf, running);
						
						}
						*running = new_running;
					}
				}
			}
	
		}

		if (strcmp(operatie, "finish") == 0) {
			finished(ca, running, &t_finish);
			fprintf(out_file, "Total time: %d\n", t_finish);
		}
	}

	if (running) {
	DistrS(&(running->s));	
	free(running);
}

	DistrQ(ca);
	DistrQ(cf);
	distrugeListaMem(&LM);
	distrugeListaPid(&LP);
	fclose(in_file);
	fclose(out_file);

	return 0;
}