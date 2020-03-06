/*Ciobanu Alin-Matei 315CB*/
#include "biblioteca.h"

int VidaQ(void *c) {
	TC coada = (TCoada*)c;
	if (coada->ic == NULL)
		return 1; // coada vida;
	return 0;
}

void *InitQ(size_t d) {
	TC a = (TC)malloc(sizeof(TCoada));
	if (!a) return NULL;
	a->lgc = 0;
	a->dime = d;
	a->ic = NULL;
	a->sc = NULL;

	return (void*)a;
}

int IntrQ(void *c, void *ae) {
	TLG aux;
	TC coada = (TCoada*)c;
	aux = AlocCelulaG(ae, coada->dime);
	if (!aux) return 0;

	if (coada->sc == NULL) {
		coada->sc = aux;
		coada->ic = coada->sc;
	} else {
		coada->sc->urm = aux;
		coada->sc = aux;
	}
	coada->lgc++;
	return 1;
}

int ExtrQ(void *c, void *ae) {
	TLG aux;
	TC coada = (TCoada*)c;
	if (coada->ic == NULL && coada->sc == NULL) return 0;
	if (coada->ic == coada->sc) coada->sc = NULL;
	aux = coada->ic;
	coada->ic = aux->urm;
	memcpy(ae, aux->info, coada->dime);
	free(aux->info);
	free(aux);
	coada->lgc--;
	return 1;
}

void DistrQ(void *c) {
	TLG aux;
	TC coada = (TC)c;
	TProces *p;
	while (coada->lgc > 0) {
		aux = coada->ic;
		coada->ic = aux->urm;
		p = (TProces*)aux->info;
		DistrS(&(p->s));
		free(aux->info);
		free(aux);
		coada->lgc--;
	}
	coada->ic = coada->sc = NULL;
	free(c);
}
