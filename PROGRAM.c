#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"  // pour sauvegarder les images
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// effectue le chargement et renvoie l'image rgb
unsigned char *chargerImage(char *nomFichier, int *largeur, int *hauteur) {
    int canaux;
    unsigned char *img = stbi_load(nomFichier, largeur, hauteur, &canaux, 3); // force rgb
    if (img == NULL) {
        printf(" Échec du chargement de l'image\n");
        return NULL;
    }
    printf(" Image chargee avec succes: %d x %d pixels\n", *largeur, *hauteur);

    // Sauvegarde automatique de la matrice RGB
    char nomMatriceRGB[200];
    // Ajout du nom de la photo dans le nom du fichier
    sprintf(nomMatriceRGB, "matrice_rgb_%s.txt", nomFichier);

    FILE *out = fopen(nomMatriceRGB, "w");
    if (out == NULL) {
        printf(" Échec de creation du fichier matrice RGB\n");
    } else {
        int i = 0;
        while (i < *hauteur) {
            int j = 0;
            while (j < *largeur) {
                int idx = (i * (*largeur) + j) * 3;
                int r = img[idx];
                int g = img[idx + 1];
                int b = img[idx + 2];
                fprintf(out, "(%3d,%3d,%3d) ", r, g, b);
                j = j + 1;
            }
            fprintf(out, "\n");
            i = i + 1;
        }
        fclose(out);
        printf(" Matrice RGB sauvegardee automatiquement dans %s\n", nomMatriceRGB);
    }

    return img;
}

// sauvegarde les valeurs rgb dans un fichier
void sauvegarderMatriceRGB(unsigned char *img, int largeur, int hauteur, char *nomFichierOriginal) {
    char fichierSortie[200];
    sprintf(fichierSortie, "matrice_rgb_%s.txt", nomFichierOriginal);

    FILE *out = fopen(fichierSortie, "w");
    if (out == NULL) {
        printf(" Échec de creation du fichier de sortie\n");
        return;
    }
    int i = 0;
    while (i < hauteur) {
        int j = 0;
        while (j < largeur) {
            int idx = (i * largeur + j) * 3;
            int r = img[idx];
            int g = img[idx + 1];
            int b = img[idx + 2];
            fprintf(out, "(%3d,%3d,%3d) ", r, g, b);
            j = j + 1;
        }
        fprintf(out, "\n");
        i = i + 1;
    }
    fclose(out);
    printf(" Matrice RGB sauvegardee dans %s\n", fichierSortie);
}

// sauvegarde la matrice rgb inversee dans un fichier
void sauvegarderMatriceRGBInversee(unsigned char *img, int largeur, int hauteur, char *nomFichierOriginal) {
    char fichierSortie[200];
    sprintf(fichierSortie, "matrice_inverse_%s.txt", nomFichierOriginal);

    FILE *out = fopen(fichierSortie, "w");
    if (out == NULL) {
        printf(" Échec de creation du fichier inverse\n");
        return;
    }
    int i = 0;
    while (i < hauteur) {
        int j = 0;
        while (j < largeur) {
            int idx = (i * largeur + j) * 3;
            int r = 255 - img[idx];
            int g = 255 - img[idx + 1];
            int b = 255 - img[idx + 2];
            fprintf(out, "(%3d,%3d,%3d) ", r, g, b);
            j = j + 1;
        }
        fprintf(out, "\n");
        i = i + 1;
    }
    fclose(out);
    printf(" Matrice inversee sauvegardee dans %s\n", fichierSortie);
}

// Cree une copie de l'image pour eviter de modifier l'originale
unsigned char *clonerImage(unsigned char *img, int largeur, int hauteur) {
    unsigned char *clone = (unsigned char*)malloc(largeur * hauteur * 3);
    if (clone == NULL) {
        printf(" Échec d'allocation de memoire pour l'image\n");
        return NULL;
    }
    memcpy(clone, img, largeur * hauteur * 3);
    return clone;
}

// cree et sauvegarde l'image inversee en fichier .jpg
void creerImageInversee(char *nomOriginal, unsigned char *img, int largeur, int hauteur) {
    char nouveauNom[200];
    sprintf(nouveauNom, "inverse_%s", nomOriginal);

    // Cree une copie pour eviter de modifier l'originale
    unsigned char *imgInv = clonerImage(img, largeur, hauteur);
    if (imgInv == NULL) return;

    int i = 0;
    while (i < hauteur) {
        int j = 0;
        while (j < largeur) {
            int idx = (i * largeur + j) * 3;
            imgInv[idx] = 255 - imgInv[idx];
            imgInv[idx + 1] = 255 - imgInv[idx + 1];
            imgInv[idx + 2] = 255 - imgInv[idx + 2];
            j = j + 1;
        }
        i = i + 1;
    }

    if (stbi_write_jpg(nouveauNom, largeur, hauteur, 3, imgInv, 100)) {
        printf(" Image inversee sauvegardee sous %s\n", nouveauNom);
    } else {
        printf(" Échec de sauvegarde de l'image inversee\n");
    }

    free(imgInv);
}

// sauvegarde la matrice en niveaux de gris
void sauvegarderMatriceNiveauxGris(unsigned char *img, int largeur, int hauteur, char *nomFichierOriginal) {
    char nomMatrice[200];
    sprintf(nomMatrice, "matrice_niveauxGris_%s.txt", nomFichierOriginal);

    FILE *matriceGris = fopen(nomMatrice, "w");

    if (matriceGris == NULL) {
        printf(" Échec de creation du fichier de matrice niveaux de gris\n");
        return;
    }

    int i = 0;
    while (i < hauteur) {
        int j = 0;
        while (j < largeur) {
            int idx = (i * largeur + j) * 3;

            // Calcule la valeur en niveaux de gris avec la formule standard de luminance
            int niveauGris = (int)(0.299 * img[idx] + 0.587 * img[idx + 1] + 0.114 * img[idx + 2]);

            // Écrit dans la matrice en niveaux de gris
            fprintf(matriceGris, "(%3d) ", niveauGris);

            j = j + 1;
        }
        fprintf(matriceGris, "\n");
        i = i + 1;
    }

    fclose(matriceGris);
    printf(" Matrice en niveaux de gris sauvegardee dans %s\n", nomMatrice);
}

// cree et sauvegarde l'image en niveaux de gris
void creerImageNiveauxGris(char *nomOriginal, unsigned char *img, int largeur, int hauteur) {
    char nouveauNom[200];
    sprintf(nouveauNom, "niveauxGris_%s", nomOriginal);

    // Cree une nouvelle image en niveaux de gris
    unsigned char *imgGris = (unsigned char*)malloc(largeur * hauteur * 3);

    if (imgGris == NULL) {
        printf(" Échec d'allocation de memoire pour l'image en niveaux de gris\n");
        return;
    }

    int i = 0;
    while (i < hauteur) {
        int j = 0;
        while (j < largeur) {
            int idx = (i * largeur + j) * 3;

            // Calcule la valeur en niveaux de gris
            int niveauGris = (int)(0.299 * img[idx] + 0.587 * img[idx + 1] + 0.114 * img[idx + 2]);

            // Definit les pixels de l'image en niveaux de gris
            imgGris[idx] = niveauGris;
            imgGris[idx + 1] = niveauGris;
            imgGris[idx + 2] = niveauGris;

            j = j + 1;
        }
        i = i + 1;
    }

    // Sauvegarde l'image
    if (stbi_write_jpg(nouveauNom, largeur, hauteur, 3, imgGris, 100)) {
        printf(" Image en niveaux de gris sauvegardee sous %s\n", nouveauNom);
    } else {
        printf(" Échec de sauvegarde de l'image en niveaux de gris\n");
    }

    free(imgGris);
}

// sauvegarde la matrice noir et blanc
void sauvegarderMatriceNoirBlanc(unsigned char *img, int largeur, int hauteur, char *nomFichierOriginal) {
    char nomMatrice[200];
    sprintf(nomMatrice, "matrice_noirBlanc_%s.txt", nomFichierOriginal);

    FILE *matriceNB = fopen(nomMatrice, "w");

    if (matriceNB == NULL) {
        printf(" Échec de creation du fichier de matrice noir et blanc\n");
        return;
    }

    int i = 0;
    while (i < hauteur) {
        int j = 0;
        while (j < largeur) {
            int idx = (i * largeur + j) * 3;

            // Calcule la valeur en niveaux de gris
            int niveauGris = (int)(0.299 * img[idx] + 0.587 * img[idx + 1] + 0.114 * img[idx + 2]);

            // Determine la valeur noir/blanc basee sur un seuil
            int nb = (niveauGris >= 128) ? 255 : 0;

            // Écrit dans la matrice noir et blanc
            fprintf(matriceNB, "(%3d) ", nb);

            j = j + 1;
        }
        fprintf(matriceNB, "\n");
        i = i + 1;
    }

    fclose(matriceNB);
    printf(" Matrice noir et blanc sauvegardee dans %s\n", nomMatrice);
}

// cree et sauvegarde l'image en noir et blanc
void creerImageNoirBlanc(char *nomOriginal, unsigned char *img, int largeur, int hauteur) {
    char nouveauNom[200];
    sprintf(nouveauNom, "noirBlanc_%s", nomOriginal);

    // Cree une nouvelle image noir et blanc
    unsigned char *imgNB = (unsigned char*)malloc(largeur * hauteur * 3);

    if (imgNB == NULL) {
        printf(" Échec d'allocation de memoire pour l'image noir et blanc\n");
        return;
    }

    int i = 0;
    while (i < hauteur) {
        int j = 0;
        while (j < largeur) {
            int idx = (i * largeur + j) * 3;

            // Calcule la valeur en niveaux de gris
            int niveauGris = (int)(0.299 * img[idx] + 0.587 * img[idx + 1] + 0.114 * img[idx + 2]);

            // Determine la valeur noir/blanc basee sur un seuil
            int nb = (niveauGris >= 128) ? 255 : 0;

            // Definit les pixels de l'image noir et blanc
            imgNB[idx] = nb;
            imgNB[idx + 1] = nb;
            imgNB[idx + 2] = nb;

            j = j + 1;
        }
        i = i + 1;
    }

    // Sauvegarde l'image
    if (stbi_write_jpg(nouveauNom, largeur, hauteur, 3, imgNB, 100)) {
        printf(" Image noir et blanc sauvegardee sous %s\n", nouveauNom);
    } else {
        printf(" Échec de sauvegarde de l'image noir et blanc\n");
    }

    free(imgNB);
}

// sauvegarde la matrice de l'image pivotee
void sauvegarderMatriceRotation(unsigned char *imgRotation, int nouvelleLargeur, int nouvelleHauteur, char *nomFichierOriginal, float angle) {
    char nomMatrice[200];
    // Inclure l'angle de rotation dans le nom du fichier
    sprintf(nomMatrice, "matrice_rotation_%.0f_deg_%s.txt", angle, nomFichierOriginal);

    FILE *matriceRot = fopen(nomMatrice, "w");
    if (matriceRot == NULL) {
        printf(" Échec de creation du fichier de matrice rotation\n");
        return;
    }

    int i = 0;
    while (i < nouvelleHauteur) {
        int j = 0;
        while (j < nouvelleLargeur) {
            int idx = (i * nouvelleLargeur + j) * 3;
            int r = imgRotation[idx];
            int g = imgRotation[idx + 1];
            int b = imgRotation[idx + 2];
            fprintf(matriceRot, "(%3d,%3d,%3d) ", r, g, b);
            j = j + 1;
        }
        fprintf(matriceRot, "\n");
        i = i + 1;
    }

    fclose(matriceRot);
    printf(" Matrice de rotation (%.0f degres) sauvegardee dans %s\n", angle, nomMatrice);
}

// cree et sauvegarde l'image pivotee
void creerImageRotation(char *nomOriginal, unsigned char *img, int largeur, int hauteur, float angle) {
    // Convertir l'angle en radians
    float angleRad = angle * M_PI / 180.0;
    float cosAngle = cos(angleRad);
    float sinAngle = sin(angleRad);

    // Calculer les dimensions de l'image après rotation
    int nouvelleLargeur = (int)(fabs(largeur * cosAngle) + fabs(hauteur * sinAngle)) + 1;
    int nouvelleHauteur = (int)(fabs(hauteur * cosAngle) + fabs(largeur * sinAngle)) + 1;

    // Creer la nouvelle image
    unsigned char *imgRotation = (unsigned char*)malloc(nouvelleLargeur * nouvelleHauteur * 3);
    if (imgRotation == NULL) {
        printf(" Échec d'allocation de memoire pour l'image pivotee\n");
        return;
    }

    // Initialiser la nouvelle image avec des pixels noirs
    memset(imgRotation, 0, nouvelleLargeur * nouvelleHauteur * 3);

    // Centre de l'image originale
    float cx = largeur / 2.0f;
    float cy = hauteur / 2.0f;

    // Centre de la nouvelle image
    float ncx = nouvelleLargeur / 2.0f;
    float ncy = nouvelleHauteur / 2.0f;

    int i, j;
    for (i = 0; i < nouvelleHauteur; i++) {
        for (j = 0; j < nouvelleLargeur; j++) {
            // Coordonnees relatives au centre de la nouvelle image
            float x = j - ncx;
            float y = i - ncy;

            // Appliquer la rotation inverse
            float srcX = x * cosAngle + y * sinAngle + cx;
            float srcY = -x * sinAngle + y * cosAngle + cy;

            // Verifier si le pixel source est dans les limites de l'image originale
            if (srcX >= 0 && srcX < largeur && srcY >= 0 && srcY < hauteur) {
                // Interpolation bilineaire
                int x0 = (int)srcX;
                int y0 = (int)srcY;
                int x1 = x0 + 1;
                int y1 = y0 + 1;

                // S'assurer que les coordonnees sont dans les limites
                x1 = (x1 >= largeur) ? largeur - 1 : x1;
                y1 = (y1 >= hauteur) ? hauteur - 1 : y1;

                // Poids pour l'interpolation
                float dx = srcX - x0;
                float dy = srcY - y0;

                // Indices des pixels dans l'image originale
                int idx00 = (y0 * largeur + x0) * 3;
                int idx01 = (y0 * largeur + x1) * 3;
                int idx10 = (y1 * largeur + x0) * 3;
                int idx11 = (y1 * largeur + x1) * 3;

                // Indice du pixel dans la nouvelle image
                int newIdx = (i * nouvelleLargeur + j) * 3;

                // Interpolation pour chaque canal de couleur
                for (int c = 0; c < 3; c++) {
                    float val = (1 - dx) * (1 - dy) * img[idx00 + c] +
                                dx * (1 - dy) * img[idx01 + c] +
                                (1 - dx) * dy * img[idx10 + c] +
                                dx * dy * img[idx11 + c];

                    imgRotation[newIdx + c] = (unsigned char)val;
                }
            }
        }
    }

    // Sauvegarder l'image pivotee avec l'angle dans le nom du fichier
    char nouveauNom[200];
    sprintf(nouveauNom, "rotation_%.0f_deg_%s", angle, nomOriginal);

    if (stbi_write_jpg(nouveauNom, nouvelleLargeur, nouvelleHauteur, 3, imgRotation, 100)) {
        printf(" Image pivotee (%.0f degres) sauvegardee sous %s\n", angle, nouveauNom);
    } else {
        printf(" Échec de sauvegarde de l'image pivotee\n");
    }

    // Sauvegarder la matrice avec l'angle
    sauvegarderMatriceRotation(imgRotation, nouvelleLargeur, nouvelleHauteur, nomOriginal, angle);

    free(imgRotation);
}

// sauvegarde la matrice de l'image inversee horizontalement
void sauvegarderMatriceFlip(unsigned char *imgFlip, int largeur, int hauteur, char *nomFichierOriginal) {
    char nomMatrice[200];
    sprintf(nomMatrice, "matrice_flip_horizontal_%s.txt", nomFichierOriginal);

    FILE *matriceFlip = fopen(nomMatrice, "w");
    if (matriceFlip == NULL) {
        printf(" Échec de creation du fichier de matrice flip\n");
        return;
    }

    int i = 0;
    while (i < hauteur) {
        int j = 0;
        while (j < largeur) {
            int idx = (i * largeur + j) * 3;
            int r = imgFlip[idx];
            int g = imgFlip[idx + 1];
            int b = imgFlip[idx + 2];
            fprintf(matriceFlip, "(%3d,%3d,%3d) ", r, g, b);
            j = j + 1;
        }
        fprintf(matriceFlip, "\n");
        i = i + 1;
    }

    fclose(matriceFlip);
    printf(" Matrice de flip horizontal sauvegardee dans %s\n", nomMatrice);
}

// cree et sauvegarde l'image inversee horizontalement (gauche à droite)
void creerImageFlip(char *nomOriginal, unsigned char *img, int largeur, int hauteur) {
    char nouveauNom[200];
    sprintf(nouveauNom, "flip_horizontal_%s", nomOriginal);

    // Creer une nouvelle image pour le flip horizontal
    unsigned char *imgFlip = (unsigned char*)malloc(largeur * hauteur * 3);
    if (imgFlip == NULL) {
        printf(" Échec d'allocation de memoire pour l'image inversee horizontalement\n");
        return;
    }

    int i = 0;
    while (i < hauteur) {
        int j = 0;
        while (j < largeur) {
            // Indice du pixel original
            int idxSrc = (i * largeur + j) * 3;

            // Indice du pixel inverse (même ligne, colonne inversee)
            int idxDst = (i * largeur + (largeur - 1 - j)) * 3;

            // Copier les valeurs RGB
            imgFlip[idxDst] = img[idxSrc];
            imgFlip[idxDst + 1] = img[idxSrc + 1];
            imgFlip[idxDst + 2] = img[idxSrc + 2];

            j = j + 1;
        }
        i = i + 1;
    }

    // Sauvegarder l'image
    if (stbi_write_jpg(nouveauNom, largeur, hauteur, 3, imgFlip, 100)) {
        printf(" Image inversee horizontalement sauvegardee sous %s\n", nouveauNom);
    } else {
        printf(" Échec de sauvegarde de l'image inversee horizontalement\n");
    }

    // Sauvegarder la matrice
    sauvegarderMatriceFlip(imgFlip, largeur, hauteur, nomOriginal);

    free(imgFlip);
}

void afficherBienvenue() {
    printf("+----------------------------------------+\n");
    printf("|                                        |\n");
    printf("|     PROGRAMME DE TRAITEMENT D'IMAGE    |\n");
    printf("|                                        |\n");
    printf("+----------------------------------------+\n");
    printf("\n Bienvenue dans le Programme de Traitement d'Image !\n");
    printf(" Ce programme vous permet d'appliquer divers effets à vos images.\n\n");
}

void afficherMenu() {
    printf("\n");
    printf("+----------------------------------------+\n");
    printf("|            OPTIONS DU MENU             |\n");
    printf("+----------------------------------------+\n");
    printf("| 1. Inverser l'image (r,v,b)            |\n");
    printf("| 2. Traiter en Niveaux de Gris          |\n");
    printf("| 3. Traiter en Noir et Blanc            |\n");
    printf("| 4. Pivoter l'image                     |\n");
    printf("| 5. Inverser horizontalement (flip)     |\n");
    printf("| 6. Traiter Toutes les Options          |\n");
    printf("| 0. Quitter                             |\n");
    printf("+----------------------------------------+\n");
    printf(" Entrez votre choix: ");
}

// Fonction pour traiter toutes les options à la fois
void traiterToutesOptions(char *nomFichier, unsigned char *img, int largeur, int hauteur) {
    printf("\n === Traitement de Toutes les Options ===\n");

    // 1. Inverser l'image (r,g,b)
    printf("\n --- Creation de l'Image Inversee ---\n");
    sauvegarderMatriceRGBInversee(img, largeur, hauteur, nomFichier);
    creerImageInversee(nomFichier, img, largeur, hauteur);

    // 2. Traiter en Niveaux de Gris
    printf("\n --- Traitement en Niveaux de Gris ---\n");
    sauvegarderMatriceNiveauxGris(img, largeur, hauteur, nomFichier);
    creerImageNiveauxGris(nomFichier, img, largeur, hauteur);

    // 3. Traiter en Noir et Blanc
    printf("\n --- Traitement en Noir et Blanc ---\n");
    sauvegarderMatriceNoirBlanc(img, largeur, hauteur, nomFichier);
    creerImageNoirBlanc(nomFichier, img, largeur, hauteur);

    // 4. Pivoter l'image (90 degres par defaut)
    printf("\n --- Rotation de l'Image (90 degres) ---\n");
    creerImageRotation(nomFichier, img, largeur, hauteur, 90.0);

    // 5. Inverser horizontalement
    printf("\n --- Inversion Horizontale de l'Image ---\n");
    creerImageFlip(nomFichier, img, largeur, hauteur);

    printf("\n Tous les traitements sont termines !\n");
}

int main() {
    afficherBienvenue();
    char nomFichier[100];
    printf(" Entrez le nom du fichier image (avec .jpg): ");
    scanf("%s", nomFichier);
    printf("\n");

    int largeur, hauteur;
    unsigned char *img = chargerImage(nomFichier, &largeur, &hauteur);
    if (img == NULL) {
        return 1;
    }

    int choix;
    do {
        afficherMenu();
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                printf("\n === Creation de l'Image Inversee ===\n");
                sauvegarderMatriceRGBInversee(img, largeur, hauteur, nomFichier);
                creerImageInversee(nomFichier, img, largeur, hauteur);
                break;

            case 2:
                printf("\n === Traitement en Niveaux de Gris ===\n");
                sauvegarderMatriceNiveauxGris(img, largeur, hauteur, nomFichier);
                creerImageNiveauxGris(nomFichier, img, largeur, hauteur);
                break;

            case 3:
                printf("\n === Traitement en Noir et Blanc ===\n");
                sauvegarderMatriceNoirBlanc(img, largeur, hauteur, nomFichier);
                creerImageNoirBlanc(nomFichier, img, largeur, hauteur);
                break;

            case 4:
                printf("\n === Rotation de l'Image ===\n");
                {
                    float angle;
                    printf(" Entrez l'angle de rotation en degres: ");
                    scanf("%f", &angle);
                    creerImageRotation(nomFichier, img, largeur, hauteur, angle);
                }
                break;

            case 5:
                printf("\n === Inversion Horizontale de l'Image ===\n");
                creerImageFlip(nomFichier, img, largeur, hauteur);
                break;

            case 6:
                // Utiliser la fonction dediee pour traiter toutes les options
                traiterToutesOptions(nomFichier, img, largeur, hauteur);
                break;

            case 0:
                printf("\n Sortie du programme. Au revoir !\n");
                break;

            default:
                printf("\n Choix invalide. Veuillez reessayer.\n");
        }

    } while (choix != 0);

    stbi_image_free(img);
    return 0;
}
