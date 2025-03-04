#include <stdio.h>
#include <stdlib.h>





typedef enum { KIRMIZI, SIYAH } Renk;

typedef struct Dugum {
    int veri;
    Renk renk;
    struct Dugum *sol, *sag, *ebeveyn;
} Dugum;

typedef struct {
    Dugum *kok;
} KSAgac;

Dugum* dugumOlustur(int veri) {
    Dugum *yeniDugum = (Dugum *)malloc(sizeof(Dugum));
    yeniDugum->veri = veri;
    yeniDugum->renk = KIRMIZI; // Yeni düğümler başlangıçta kırmızı renkte olur
    yeniDugum->sol = yeniDugum->sag = yeniDugum->ebeveyn = NULL;
    return yeniDugum;
}

void solaDondur(KSAgac *agac, Dugum *x) {
    Dugum *y = x->sag;
    x->sag = y->sol;
    if (y->sol != NULL) {
        y->sol->ebeveyn = x;
    }
    y->ebeveyn = x->ebeveyn;
    if (x->ebeveyn == NULL) {
        agac->kok = y;
    } else if (x == x->ebeveyn->sol) {
        x->ebeveyn->sol = y;
    } else {
        x->ebeveyn->sag = y;
    }
    y->sol = x;
    x->ebeveyn = y;
}

void sagaDondur(KSAgac *agac, Dugum *y) {
    Dugum *x = y->sol;
    y->sol = x->sag;
    if (x->sag != NULL) {
        x->sag->ebeveyn = y;
    }
    x->ebeveyn = y->ebeveyn;
    if (y->ebeveyn == NULL) {
        agac->kok = x;
    } else if (y == y->ebeveyn->sag) {
        y->ebeveyn->sag = x;
    } else {
        y->ebeveyn->sol = x;
    }
    x->sag = y;
    y->ebeveyn = x;
}

void ihlalDuzelt(KSAgac *agac, Dugum *z) {
    while (z != agac->kok && z->ebeveyn->renk == KIRMIZI) {
        if (z->ebeveyn == z->ebeveyn->ebeveyn->sol) {
            Dugum *y = z->ebeveyn->ebeveyn->sag;
            if (y != NULL && y->renk == KIRMIZI) {
                z->ebeveyn->renk = SIYAH;
                y->renk = SIYAH;
                z->ebeveyn->ebeveyn->renk = KIRMIZI;
                z = z->ebeveyn->ebeveyn;
            } else {
                if (z == z->ebeveyn->sag) {
                    z = z->ebeveyn;
                    solaDondur(agac, z);
                }
                z->ebeveyn->renk = SIYAH;
                z->ebeveyn->ebeveyn->renk = KIRMIZI;
                sagaDondur(agac, z->ebeveyn->ebeveyn);
            }
        } else {
            Dugum *y = z->ebeveyn->ebeveyn->sol;
            if (y != NULL && y->renk == KIRMIZI) {
                z->ebeveyn->renk = SIYAH;
                y->renk = SIYAH;
                z->ebeveyn->ebeveyn->renk = KIRMIZI;
                z = z->ebeveyn->ebeveyn;
            } else {
                if (z == z->ebeveyn->sol) {
                    z = z->ebeveyn;
                    sagaDondur(agac, z);
                }
                z->ebeveyn->renk = SIYAH;
                z->ebeveyn->ebeveyn->renk = KIRMIZI;
                solaDondur(agac, z->ebeveyn->ebeveyn);
            }
        }
    }
    agac->kok->renk = SIYAH;
}

void ekle(KSAgac *agac, int veri) {
    Dugum *yeniDugum = dugumOlustur(veri);
    if (agac->kok == NULL) {
        yeniDugum->renk = SIYAH;
        agac->kok = yeniDugum;
    } else {
        Dugum *guncel = agac->kok;
        Dugum *ebeveyn = NULL;
        while (guncel != NULL) {
            ebeveyn = guncel;
            if (veri < guncel->veri) {
                guncel = guncel->sol;
            } else {
                guncel = guncel->sag;
            }
        }
        yeniDugum->ebeveyn = ebeveyn;
        if (veri < ebeveyn->veri) {
            ebeveyn->sol = yeniDugum;
        } else {
            ebeveyn->sag = yeniDugum;
        }
        ihlalDuzelt(agac, yeniDugum);
    }
}

Dugum* minDegerDugum(Dugum* dugum) {
    Dugum* guncel = dugum;
    while (guncel->sol != NULL) {
        guncel = guncel->sol;
    }
    return guncel;
}

void silmeIhlaliDuzelt(KSAgac *agac, Dugum *x) {
    while (x != agac->kok && x->renk == SIYAH) {
        if (x == x->ebeveyn->sol) {
            Dugum *w = x->ebeveyn->sag;
            if (w->renk == KIRMIZI) {
                w->renk = SIYAH;
                x->ebeveyn->renk = KIRMIZI;
                solaDondur(agac, x->ebeveyn);
                w = x->ebeveyn->sag;
            }
            if (w->sol->renk == SIYAH && w->sag->renk == SIYAH) {
                w->renk = KIRMIZI;
                x = x->ebeveyn;
            } else {
                if (w->sag->renk == SIYAH) {
                    w->sol->renk = SIYAH;
                    w->renk = KIRMIZI;
                    sagaDondur(agac, w);
                    w = x->ebeveyn->sag;
                }
                w->renk = x->ebeveyn->renk;
                x->ebeveyn->renk = SIYAH;
                w->sag->renk = SIYAH;
                solaDondur(agac, x->ebeveyn);
                x = agac->kok;
            }
        } else {
            Dugum *w = x->ebeveyn->sol;
            if (w->renk == KIRMIZI) {
                w->renk = SIYAH;
                x->ebeveyn->renk = KIRMIZI;
                sagaDondur(agac, x->ebeveyn);
                w = x->ebeveyn->sol;
            }
            if (w->sag->renk == SIYAH && w->sol->renk == SIYAH) {
                w->renk = KIRMIZI;
                x = x->ebeveyn;
            } else {
                if (w->sol->renk == SIYAH) {
                    w->sag->renk = SIYAH;
                    w->renk = KIRMIZI;
                    solaDondur(agac, w);
                    w = x->ebeveyn->sol;
                }
                w->renk = x->ebeveyn->renk;
                x->ebeveyn->renk = SIYAH;
                w->sol->renk = SIYAH;
                sagaDondur(agac, x->ebeveyn);
                x = agac->kok;
            }
        }
    }
    x->renk = SIYAH;
}

void dugumSil(KSAgac *agac, int veri) {
    Dugum *z = agac->kok;
    Dugum *x, *y;
    while (z != NULL) {
        if (z->veri == veri) {
            break;
        } else if (z->veri < veri) {
            z = z->sag;
        } else {
            z = z->sol;
        }
    }
    if (z == NULL) {
        printf("Eleman agacta bulunamadi\n");
        return;
    }
    y = z;
    Renk y_orijinal_renk = y->renk;
    if (z->sol == NULL) {
        x = z->sag;
        if (z->ebeveyn == NULL) {
            agac->kok = x;
        } else if (z == z->ebeveyn->sol) {
            z->ebeveyn->sol = x;
        } else {
            z->ebeveyn->sag = x;
        }
        if (x != NULL) {
            x->ebeveyn = z->ebeveyn;
        }
    } else if (z->sag == NULL) {
        x = z->sol;
        if (z->ebeveyn == NULL) {
            agac->kok = x;
        } else if (z == z->ebeveyn->sol) {
            z->ebeveyn->sol = x;
        } else {
            z->ebeveyn->sag = x;
        }
        if (x != NULL) {
            x->ebeveyn = z->ebeveyn;
        }
    } else {
        y = minDegerDugum(z->sag);
        y_orijinal_renk = y->renk;
        x = y->sag;
        if (y->ebeveyn == z) {
            if (x != NULL) {
                x->ebeveyn = y;
            }
        } else {
            if (y->ebeveyn != NULL) {
                if (y == y->ebeveyn->sol) {
                    y->ebeveyn->sol = x;
                } else {
                    y->ebeveyn->sag = x;
                }
            }
            if (x != NULL) {
                x->ebeveyn = y->ebeveyn;
            }
            y->sag = z->sag;
            if (z->sag != NULL) {
                z->sag->ebeveyn = y;
            }
        }
        if (z->ebeveyn == NULL) {
            agac->kok = y;
        } else if (z == z->ebeveyn->sol) {
            z->ebeveyn->sol = y;
        } else {
            z->ebeveyn->sag = y;
        }
        y->ebeveyn = z->ebeveyn;
        y->renk = z->renk;
        y->sol = z->sol;
        if (z->sol != NULL) {
            z->sol->ebeveyn = y;
        }
    }
    if (y_orijinal_renk == SIYAH) {
        silmeIhlaliDuzelt(agac, x);
    }
    free(z);
}

void agacYazdirHelper(Dugum *kok, int bosluk) {
    int SAYI = 10;
    if (kok == NULL) {
        return;
    }
    bosluk += SAYI;
    agacYazdirHelper(kok->sag, bosluk);
    printf("\n");
    for (int i = SAYI; i < bosluk; i++) {
        printf(" ");
    }
    printf("%d", kok->veri);
    if (kok->renk == KIRMIZI) {
        printf("(K)\n");
    } else {
        printf("(S)\n");
    }
    agacYazdirHelper(kok->sol, bosluk);
}

void agacYazdir(KSAgac *agac) {
    agacYazdirHelper(agac->kok, 0);
}

int main() {
    KSAgac agac = {NULL};
    int secim, deger;

    while (1) {
        printf("\n RED-BLACK Agac Islemleri Menusu:\n");
        printf("1. Ekle\n");
        printf("2. Sil\n");
        printf("3. Agaci Yazdir\n");
        printf("4. Cikis\n");
        printf("Seciminizi girin: ");
        scanf("%d", &secim);

        switch (secim) {
            case 1:
                printf("Eklenecek degeri girin: ");
                scanf("%d", &deger);
                ekle(&agac, deger);
                break;
            case 2:
                printf("Silinecek degeri girin: ");
                scanf("%d", &deger);
                dugumSil(&agac, deger);
                break;
            case 3:
                printf("Mevcut Kirmizi-Siyah Agac:\n");
                agacYazdir(&agac);
                break;
            case 4:
                exit(0);
            default:
                printf("Gecersiz secim! Lutfen tekrar deneyin.\n");
        }
    }

    return 0;
}

