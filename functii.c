/*Ciobanu Alin-Matei 315CB*/
#include "biblioteca.h"

void *add(void *ca, TProces p) {
	int ok = 1;
	TProces x;
	void *caux;
	caux = InitQ(sizeof(TProces));
	if (!caux) return NULL;
	if (VidaQ(ca) == 1) {
		DistrQ(ca);
		IntrQ(caux, &p);
		return caux;
	}
	while (ExtrQ(ca, &x)) {
		if (p.prioritate > x.prioritate) {
			IntrQ(caux, &p);
			ok = 0; 
			break;
    	}

		if (p.prioritate == x.prioritate) {
			if (p.timp_ramas < x.timp_ramas) {
				IntrQ(caux, &p);
				ok = 0;
				break;
			} 
			else if (p.timp_ramas == x.timp_ramas) {
				if (p.PID < x.PID) {
					IntrQ(caux, &p);
					ok=0;
					break;
				}
			}
		}
		IntrQ(caux, &x);
	}

	if (ok == 0) {
		IntrQ(caux, &x);
	} else {
		IntrQ(caux, &p);
	}

	while (ExtrQ(ca, &x)) {
		IntrQ(caux, &x);
	}

	DistrQ(ca);
	return caux;
}


void afisM(void *ae, FILE *out_file) {
	TProces *p = ((TProces*)ae);
	fprintf(out_file, "Process created successfully: PID: %d, Memory starts at 0x%x.\n", p->PID, p->mem_start);
}


void afis_add(TLG L, int PID, FILE *out_file, void (*afisEl)(void *, FILE *)) {
	TProces *p;
	for (; L; L = L->urm) {
		p = (TProces*)(L->info);
		if (p->PID == PID) {
			afisEl((void*)p, out_file);
		}
	}
}


void get(void *cw, void *cf, TProces *running, int PID, FILE *out_file) {
	int gasit_r = 0;
	if (running != NULL) { // caut in starea running
	if (running->PID == PID) {
		gasit_r = 1;
		fprintf(out_file, "Process %d is running (remaining_time: %d).\n",
		 PID, running->timp_ramas);
	}
}

	TProces p;
	void *caux;
	int gasit_w = 0;
	caux = InitQ(sizeof(TProces));
	while (ExtrQ(cw, &p)) {
		// caut procesul in coada waiting
		if (p.PID == PID) {
			gasit_w = 1;
			fprintf(out_file, "Process %d is waiting (remaining_time: %d).\n",
			 PID, p.timp_ramas);
			IntrQ(caux, &p);
		} else {
			IntrQ(caux, &p);
		}
	}

	while (ExtrQ(caux, &p))
		IntrQ(cw, &p);

	DistrQ(caux);

	int gasit_f = 0;
	caux = InitQ(sizeof(TProces));
	while (ExtrQ(cf, &p)) {
		// caut procesul in coada finish
		if (p.PID == PID) {
			gasit_f = 1;
			fprintf(out_file, "Process %d is finished.\n", PID);
			IntrQ(caux, &p);
		} else {
			IntrQ(caux, &p);
		}	
	}
	while (ExtrQ(caux, &p))
		IntrQ(cf, &p);

	DistrQ(caux);

	if (gasit_w == 0 && gasit_r == 0 && gasit_f == 0) {
		fprintf(out_file, "Process %d not found.\n", PID);
	}
}


void push_s(void *ca, TProces *running, int PID, int size, FILE *out_file) {
	int gasit_r = 0;
	if (running != NULL) {
 		//caut in starea running
		if (running->PID == PID) {
			gasit_r = 1;
			if (running->mem_ramasa  == 0) {
				/*procesul nu mai are memorie disponibila*/
				fprintf(out_file, "Stack overflow PID %d.\n", PID);
			} else {
				/*adaug in stiva procesului cantitatea de 4 biti
				si scad cei 4 biti din memoria procesului*/
				Push(running->s, &size);
				running->mem_ramasa -= 4;
			}
		}
	if (gasit_r == 1) return;
}

	int gasit_w = 0;
	void *caux;
	TProces p;
	caux = InitQ(sizeof(TProces));
	while (ExtrQ(ca, &p)) { // caut in coada de asteptare
		if (p.PID == PID) {
			gasit_w = 1;
			if (p.mem_ramasa == 0) {
				fprintf(out_file, "Stack overflow PID %d.\n", PID);
			} else {
			Push(p.s, &size);
			p.mem_ramasa -= 4;
		}
			IntrQ(caux, &p);
		} else {
			IntrQ(caux, &p);
		}
	}

	while (ExtrQ(caux, &p))
		IntrQ(ca, &p);

	DistrQ(caux);

	if (gasit_w == 0 && gasit_r == 0) {
		fprintf(out_file, "PID %d not found.\n", PID);
	}
}


void pop_s(void *ca, TProces *running, int PID, FILE *out_file) {
	int gasit_r = 0, size;
	if (running != NULL) {
	if (running->PID == PID) {
		gasit_r = 1;
		if (((TS)running->s)->vf == NULL) { // stiva vida
			fprintf(out_file, "Empty stack PID %d.\n", PID);
		} else {
		 	Pop(running->s, &size);
			running->mem_ramasa += 4;
		}
	}


	if (gasit_r == 1) return; }

	TProces p;
	int gasit_w = 0;
	void *caux;
	caux = InitQ(sizeof(TProces));
	while (ExtrQ(ca, &p)) {
		if (p.PID == PID) {
			gasit_w = 1;
			if (((TS)p.s)->vf == NULL) {
				IntrQ(caux, &p);
				fprintf(out_file, "Empty stack PID %d.\n", PID);
			} else {
				Pop(p.s, &size);
				p.mem_ramasa += 4;
				IntrQ(caux, &p);
			}
		} else {
			IntrQ(caux, &p);
		}
	}

	while (ExtrQ(caux, &p))
		IntrQ(ca, &p);

	DistrQ(caux);

	if (gasit_w == 0 && gasit_r == 0) {
		fprintf(out_file, "PID %d not found.\n", PID);
	}
}


void afis_stack(void *s, int PID, FILE *out_file) {
	int x;
	void *saux;
	saux = InitS(sizeof(int));
	while (Pop(s, &x)) {
		Push(saux, &x);
	}

	fprintf(out_file, "Stack of PID %d: ", PID);
	while (Pop(saux, &x)) {
		fprintf(out_file, "%d", x);
		Push(s, &x);
		if (((TStiva*)saux)->lgs > 0) fprintf(out_file, " ");
	}
	fprintf(out_file, ".\n");

	DistrS(&saux);
}

void print_stack(void *ca, TProces *running, int PID, FILE *out_file) {
	int gasit_r = 0;
	if (running != NULL) {
	if (running->PID == PID) {
		gasit_r = 1;
		if (((TStiva*)running->s)->vf == NULL) {
			fprintf(out_file, "Empty stack PID %d.\n", PID);
		} else {
			afis_stack(running->s, PID, out_file);
		}
	}

	if (gasit_r == 1) return;
}

	int gasit_w = 0;
	TProces p;
	void *caux;
	caux = InitQ(sizeof(TProces));
	while (ExtrQ(ca, &p)) {
		if (p.PID == PID) {
			gasit_w = 1;
			if (((TS)p.s)->vf == NULL) {
				fprintf(out_file, "Empty stack PID %d.\n", PID);
			} else {
				afis_stack(p.s, PID, out_file);
			}
			IntrQ(caux, &p);
		} else
			IntrQ(caux, &p);
	}

	while (ExtrQ(caux, &p))
		IntrQ(ca, &p);

	DistrQ(caux);

	if (gasit_w == 0) {
		fprintf(out_file, "PID %d not found.\n", PID);
	}
}


void print_waiting(void *ca, FILE *out_file) {
	TProces p;
	void *caux;
	caux = InitQ(sizeof(TProces));
	fprintf(out_file, "Waiting queue:\n[");
	while (ExtrQ(ca, &p)) {
		fprintf(out_file, "(%d: priority = %d, remaining_time = %d", p.PID, p.prioritate, p.timp_ramas);
		if (((TC)ca)->lgc > 0) fprintf(out_file, "),\n");
		if (((TC)ca)->lgc == 0) fprintf(out_file, ")");
		IntrQ(caux, &p);
	}
	fprintf(out_file, "]\n");

	while (ExtrQ(caux, &p))
		IntrQ(ca, &p);

	DistrQ(caux);

}


void print_finished(void *ca, FILE *out_file) {
	TProces p;
	void *caux;
	caux = InitQ(sizeof(TProces));
	fprintf(out_file, "Finished queue:\n[");
	while (ExtrQ(ca, &p)) {
		fprintf(out_file, "(%d: priority = %d, executed_time = %d", p.PID, p.prioritate, p.exec_time);
		if (((TC)ca)->lgc > 0) fprintf(out_file, "),\n");
		if (((TC)ca)->lgc == 0) fprintf(out_file, ")");
		IntrQ(caux, &p);
	}
	fprintf(out_file, "]\n");

	while (ExtrQ(caux, &p))
		IntrQ(ca, &p);

	DistrQ(caux);
}


void finished(void *ca, TProces *running, int *t) {
	/*fac suma dintre timpii ramasi la procesele din waiting si running*/
	*t = 0;
	if (running != NULL)
		if (running->timp_ramas != 0) {
			*t += running->timp_ramas;  
	}

	void *caux;
	caux = InitQ(sizeof(TProces));
	TProces p;
	while (ExtrQ(ca, &p)) {
		if (p.timp_ramas != 0) {
			*t += p.timp_ramas;
		}
		IntrQ(caux, &p);
	}

	while (ExtrQ(caux, &p)) {
		IntrQ(ca, &p);
	}

	DistrQ(caux);
}


void GenereazaListaPid(ALG ALP, int *nr) {
	/*imi genereaza o lista de pid-uri in care fiecare celula este formata
	din valoarea pid-ului si ocupat=0/1 daca pid-ul este dat unui proces*/
	TPid pid;
	pid.val = (*nr)++;
	pid.ocupat = 0;
	InsSf(ALP, &pid, sizeof(TPid));
}


void AdaugaPid(TProces *p, TLG LP) {
	/*atribuie procesului cel mai mic pid liber*/
	TPid *auxp = NULL;
	for (; LP; LP = LP->urm) { /*parcurg lista de piduri*/
		auxp = ((TPid*)LP->info);
		if (auxp->ocupat == 0) { /*il gasesc pe primul liber*/
			p->PID = auxp->val;
			auxp->ocupat = 1;
			break;
		}
	}
}


int cmpPid(void *p1, void *p2) {
	return ( ((TProces*)p1)->PID - ((TProces*)p2)->PID );
}

void eliberarePid(ALG LP, int PID) {
	TPid *p;
	TLG u;
	for (u = *LP; u; u = u->urm) {
		p = ((TPid*)u->info);
		if (p->val == PID) {
			p->ocupat = 0;
		}
	}
}


void adaugaInListaMem(ALG aL, TProces *p, int *total_size, int *ok) {
 	/*functia imi creeaza o lista de memorie a sistemului in care fiecare
 	celula este reprezentata de procesul in sine cu toate atributele lui*/
	*ok = 1;
	*total_size += p->mem_size; /*adun memoria fiecarui proces*/
	if (*total_size > 3 * 1024 * 1024) { 
		/*inseamna ca procesul nu mai are loc in memoria rezervata de sistem*/
		*ok = 0;
		return;
	}

	TLG aux, dup;
	TProces *auxp, *dupa;
	int bagat = 0, cnt = 0;
	/*initial lista este vida, primul element este inserat la inceput*/
	if (*aL == NULL) {
		auxp = p;
		auxp->mem_start = 0; /*tin minte adresa la care procesul e alocat*/
		InsIncG(aL, auxp, sizeof(TProces));
		return;
	}

	if ((*aL)->urm == NULL) {
		/*daca in lista am un singur proces care nu incepe de la adresa 0
		daca procesul curent are loc inaintea lui il inserez la inceput*/
		aux = *aL;
		auxp = ((TProces*)aux->info);
		if (p->mem_size <= auxp->mem_start) {
			p->mem_start = 0;
			InsIncG(aL, p, sizeof(TProces));
			return;
		}
	}

	for (aux = *aL, dup = aux->urm; aux && dup; aux = aux->urm, dup = dup->urm) {
		/*am mai multe procese in lista*/
		auxp = ((TProces*)aux->info);
		dupa = ((TProces*)dup->info);
		cnt++;
		if (cnt != 1) {
			/*daca nu ma aflu la primul proces din lista*/
			if (auxp->mem_start + auxp->mem_size <= dupa->mem_start) {
				/*daca exista loc intre 2 procese*/
				if (p->mem_size <= dupa->mem_start - (auxp->mem_start + auxp->mem_size)) {
					/*daca procesul curent are loc intre ele il inserez*/
					p->mem_start = auxp->mem_size + auxp->mem_start;
					InsDupaG(aL, p, sizeof(TProces), auxp, cmpPid);
					bagat = 1;
					break;
				}
			}
		} else if (cnt == 1) { /*daca ma aflu la primul proces din lista*/
			if (p->mem_size <= auxp->mem_start) {
				/*daca procesul curent are loc inainte de primul proces*/
				p->mem_start = 0;
				InsIncG(aL, p, sizeof(TProces)); /*il inserez la inceput*/
				bagat = 1;
				break;

			} else if (auxp->mem_start + auxp->mem_size <= dupa->mem_start) {
				/*altfel daca procesul are intre primul si al doilea il inserez*/
				if (p->mem_size <= dupa->mem_start - (auxp->mem_start + auxp->mem_size)) {
					p->mem_start = auxp->mem_size + auxp->mem_start;
					InsDupaG(aL, p, sizeof(TProces), auxp, cmpPid);
					bagat = 1;
					break;
				}
			}
		}
	}

	if (bagat == 0) {
		/*daca nu a avut loc niciunde il introduc la sfarsitul listei*/
		aux = *aL;
		while (aux->urm) {
			aux = aux->urm;
		}
		auxp = ((TProces*)aux->info);
		p->mem_start = auxp->mem_start + auxp->mem_size;
		InsSf(aL, p, sizeof(TProces));
	}
}


void stergeDinListaMem(ALG aL, TProces *p) {
	/*elimina procesul p din lista de memorie*/
	TLG aux, ant;
	TProces *auxp;
	if (p == NULL) return;
	for (aux = *aL, ant = NULL; aux; ant = aux, aux = aux->urm) {
		auxp = ((TProces*)aux->info);
		if (auxp == NULL) return;
		if (auxp->PID == p->PID) break;
	}

	if (aux == NULL) return;
	if (ant == NULL) *aL = aux->urm;
		else ant->urm = aux->urm;

	free(aux->info);
	free(aux);
}


void schimba(void* a, void* b) {
	TProces *p;
	TProces* a1 = (TProces*)a;
	TProces* b1 = (TProces*)b;
	p = a1;
	a1 = b1;
	b1 = p; 
}


void defragmentare(TLG LM) {
	int ok = 0;
	if (LM == NULL || LM->urm == NULL) return;
	/*sortez lista dupa PID-uri*/
	TLG aux, u = NULL;
	do {
		ok = 0;
		aux = LM;
		while (aux->urm != u) {
			if (cmpPid(aux->info, aux->urm->info) > 0) {
				schimba(aux->info, aux->urm->info);
				ok = 1;
			}
			aux = aux->urm;
		}
		u = aux;
	} while (ok);


	TProces *p, *pant;;
	TLG ant = NULL;
	for (ant = NULL, aux = LM; aux; ant = aux, aux = aux->urm) {
		/*actualizez inceputul adresei la care procesul este alocat*/
		p = (TProces*)aux->info;
		if (ant == NULL) {
			p->mem_start = 0;
		} else {
			pant = (TProces*)ant->info;
			p->mem_start = pant->mem_start + pant->mem_size;
		}
	}
}