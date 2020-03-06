/*Ciobanu Alin-Matei 315CB*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "biblioteca.h"

TLG AlocCelulaG(void *ae, size_t d) {
	TLG aux;
	aux = (TLG)malloc(sizeof(TCelulaG));
	if (!aux) return NULL;
	aux->info = malloc(d);
	if (!aux->info) {
		free(aux);
		return NULL;
	}
	memcpy(aux->info, ae, d);
	aux->urm = NULL;
	return aux;
}

void InsSf(ALG aL, void *ae, size_t d) {
	TLG aux, u = NULL;
	if (*aL != NULL) {
		u = *aL;
		while (u->urm != NULL) {
			u = u->urm;
		}
	}
	aux = AlocCelulaG(ae, d);
	if (!aux) return;
	if (u == NULL) {
		*aL = aux;
	} else {
		u->urm = aux;
	}
}

void InsIncG(ALG aL, void *ae, size_t d) {
	TLG aux;
	aux = AlocCelulaG(ae, d);
	if (!aux) return;
	aux->urm = *aL;
	*aL = aux;
}


void InsDupaG(ALG aL, void *ae, size_t d, void *aref, int (*fcmp)(void*, void*)) {
	TLG u, aux, L = *aL;
	for (; L; L = L->urm) {
		if (fcmp(L->info, aref) == 0) {
			u = L;
			break;
		}
	}

	if (*aL == NULL) return;
	aux = AlocCelulaG(ae, d);
	if (!aux) return;
	aux->urm = u->urm;
	u->urm = aux;
}

void distrugeListaMem(ALG aL) {
	TLG aux;
	while (*aL) {
		aux = *aL;
		*aL = aux->urm;
		free(aux->info);
		free(aux);
	}
}

void distrugeListaPid(ALG aL) {
	TLG aux;
	while (*aL) {
		aux = *aL;
		*aL = aux->urm;
		free(aux->info);
		free(aux);
	}
}