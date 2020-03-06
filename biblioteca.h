/*Ciobanu Alin-Matei 315CB*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct celulag {
	void *info;
	struct celulag *urm;
}TCelulaG, *TLG, **ALG;

typedef struct {
	size_t dime;
	int lgc;
	TLG ic, sc;
}TCoada, *TC, **ACoada;

typedef struct {
	size_t dime;
	int lgs;
	TLG vf;
}TStiva, *TS, **AStiva;

typedef struct {
	int ocupat;
	int val;
}TPid;

typedef struct {
	int PID;
	int prioritate;
	int exec_time;
	int timp_ramas;
	int cuanta_ramasa;
	int mem_size;
	int mem_ramasa;
	int mem_start;
	void *s;
}TProces;

/*functiiLG*/
TLG AlocCelulaG(void *ae, size_t d);
void InsSf(ALG aL, void *ae, size_t d);
void InsIncG(ALG aL, void *ae, size_t d);
void InsDupaG(ALG aL, void *ae, size_t d, void *aref, int (*fcmp)(void*, void*));
void distrugeListaMem(ALG aL);
void distrugeListaPid(ALG aL);

/*FunctiiCoada*/
void *InitQ(size_t d);
int IntrQ(void *c, void *ae);
int ExtrQ(void *c, void *ae);
int ConcatQ(void *d, void *s);
void DistrQ(void *c);
int VidaQ(void *c);

/*FunctiiStiva*/
void *InitS(size_t d);
int Push(void *s, void *ae);
int Pop(void *s, void *ae);
void DistrS(void **s);

/*functii*/
void *add(void *ca, TProces p);
void afisM(void *ae, FILE *out_file);
void afis_add(TLG L, int PID, FILE *out_file, void (*afisEl)(void *, FILE *));
void get(void *cw, void *cf, TProces *running, int PID, FILE *out_file);
void push_s(void *ca, TProces *running, int PID, int size, FILE *out_file);
void pop_s(void *ca, TProces *running, int PID, FILE *out_file);
void afis_stack(void *s, int PID, FILE *out_file);
void print_stack(void *ca, TProces *running, int PID, FILE *out_file);
void print_waiting(void *ca, FILE *out_file);
void print_finished(void *ca, FILE *out_file);
void finished(void *ca, TProces *running, int *t);
void GenereazaListaPid(ALG ALP, int *nr);
void AdaugaPid(TProces *p, TLG LP);
int cmpPid(void *p1, void *p2);
void eliberarePid(ALG LP, int PID);
void adaugaInListaMem(ALG aL, TProces *p, int *total_size, int *ok);
void stergeDinListaMem(ALG aL, TProces *p);
void schimba(void* a, void* b);
void defragmentare(TLG LM);