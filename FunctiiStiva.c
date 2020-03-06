/*Ciobanu Alin-Matei 315CB*/
#include "biblioteca.h"

void *InitS(size_t d) {
	TS s;
	s = (TStiva*)malloc(sizeof(TStiva));
	if (!s) return NULL;
	s->dime = d;
	s->lgs = 0;
	s->vf = NULL;
	return (void*)s;
}

int Push(void *s, void *ae) {
	TS stiva = (TStiva*)s;
	TLG aux;
	aux = AlocCelulaG(ae, stiva->dime);
	if (!aux) return 0;
	aux->urm = stiva->vf;
	stiva->vf = aux;
	stiva->lgs++;
	return 1;
}

int Pop(void *s, void *ae) {
	TS stiva = (TStiva*)s;
	if (stiva->vf == NULL) return 0;
	TLG aux;
	memcpy(ae, stiva->vf->info, stiva->dime);
	aux = stiva->vf;
	stiva->vf = aux->urm;
	free(aux->info);
	free(aux);
	stiva->lgs--;
	return 1;
}

void DistrS(void **s) {
	AStiva stiva = (TStiva**)s;
	TLG aux;
	while ((*stiva)->lgs > 0) {
		aux = (*stiva)->vf;
		if (aux == NULL) return;
		(*stiva)->vf = aux->urm;
		free(aux->info);
		free(aux);
		(*stiva)->lgs--;
	}
	free(*s);
}