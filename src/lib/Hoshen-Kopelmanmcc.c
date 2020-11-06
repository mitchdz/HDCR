#include "Hoshen-Kopelmanmcc.h"

#include <limits.h>

static bool checkForeground(int value, bool CGL)
{
    bool foreground;
    if (CGL) { //ComponentGrayLevel is 1 meaning black=foreground
        foreground = ( value > 0) ? true : false;
    }
    else {
        foreground = ( value < 255) ? true : false;
    }
    return foreground;
}

static int getLowestEquivalentLabel(setNode *head, int setID)
{
    labelNode *ln = getSetNode(head, setID)->labels;

    int lowestLabel = INT_MAX;
    while (ln != NULL ) {
        if (lowestLabel > ln->label)
            lowestLabel = ln->label;

        ln = ln->next;
    }

    return lowestLabel;
}

// this function does not work properly yet, it is super close. But no cigar iterativeCCL works.
void findMaximal8ConnectedForegroundComponents(IMAGE *img, uint8_t **outccM, bool CGL, int *nc, bool verbose)
{
    int r,c, i, smallestSetID, uniqueLabel = 1;

    setNode *equivalenceTable = NULL;

    int **ccM = matalloc(img->n_rows, img->n_cols, 0, 0, sizeof(int));
    for(r=0;r<img->n_rows;r++){
        for(c=0;c<img->n_cols;c++){
            ccM[r][c] = 0;
        }
    }

    int NW, N, NE, W;

    int n[4]; //neighbor
    /*  0 | 1 | 2
     *  -----------
     *  3 | * |
     *  -----------
     *    |   |
     */
    // Hoshen-Kopelman algorithm
    // first pass - assign temporary labels and record equivalences
    for (r = 1; r < img->n_rows-1; r++) { // raster scanning
        for (c = 1; c < img->n_cols-1; c++) {

            // only worry about foreground pixels
            if (!checkForeground(img->raw_bits[r][c], CGL)) continue;

            NW = ccM[r-1][c-1];   N  = ccM[r-1][ c ];   NE = ccM[r-1][c+1];
            W  = ccM[ r ][c-1];

            // get 4 neighbor pixels (8-connectivity)
            n[0] = ( NW > 0) ? NW : 0;
            n[1] = ( N  > 0) ? N  : 0;
            n[2] = ( NE > 0) ? NE : 0;
            n[3] = ( W  > 0) ? W  : 0;

            // if no unique neighbors, l current element and continue
            if ((n[0] == 0) && (n[1] == 0) && (n[2] == 0) && (n[3] == 0)) {
                ccM[r][c] = uniqueLabel;
                pushSetID(&equivalenceTable, uniqueLabel);
                uniqueLabel++;
                continue;
            }

            // else if there are neighbors, find smallest l and assign
            smallestSetID = INT_MAX;
            for (i = 0; i < 4; i++) {
                if (smallestSetID > n[i] && n[i] != 0)
                    smallestSetID = n[i];
            }
            ccM[r][c] = smallestSetID;

            // store equivalence between neighboring labels
            for (i = 0; i < 4; i++) {
                if (n[i] != 0)
                    unionEquivalenceLabels(equivalenceTable, smallestSetID, n[i]);
            }

        } // end cols
    } // end rows

    // second pass - replace each temp l by the smallest l of
    //               equivalence class

    int *setCounts = (int *)malloc(sizeof(int)*uniqueLabel);
    for (i = 0; i < uniqueLabel;i++) setCounts[i] = 0;

    int lowestEquivalentLabel = INT_MAX;
    for (r = 1; r < img->n_rows-1; r++) {
        for (c = 1; c < img->n_cols-1; c++) {

            // only worry about foreground pixels
            if (!checkForeground(img->raw_bits[r][c], CGL)) continue;

            lowestEquivalentLabel = getLowestEquivalentLabel(equivalenceTable, ccM[r][c]);

            ccM[r][c] = lowestEquivalentLabel;

            setCounts[lowestEquivalentLabel]++;
        } // end col 2nd pass
    } // end row 2nd pass

    int numSets = 0;
    for (i = 0; i < uniqueLabel;i++) {
        if (setCounts[i] > 0) numSets++;
        if (verbose) printf("set %d:%d\n",i, setCounts[i]);
    }
    free(setCounts);

    if (verbose) listEquivalencetable(equivalenceTable);

    int setVal;
    // have to copy each value of l into outccM
    for (r = 1; r < img->n_rows-1; r++) { // raster scanning
        for (c = 1; c < img->n_cols-1; c++) {
            setVal = ccM[r][c];
            outccM[r][c] = setVal;
        } // end col 2nd pass
    } // end row 2nd pass

    *nc = numSets;
    return;
}
void initializeLabelNode(labelNode *ln)
{
    ln->label = 0;
    ln->next = NULL;
}

void initializeSetNode(setNode *sn)
{
    sn->labels = NULL;
    sn->next = NULL;
    sn->setID = 0;
}

setNode *getSetNode(setNode* head, int ID)
{
    while (head != NULL) {
        if (head->setID == ID) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}


void pushLabelNode(labelNode** head, int ID)
{
    labelNode *ln = (labelNode *)malloc(sizeof(labelNode));
    initializeLabelNode(ln);
    ln->label = ID;

    ln->next = (*head);
    (*head) = ln;
}


// adds X labels into Y
void combineSetIDLabels(setNode* head, int setX, int setY)
{
    //lnX is head of setX labels which is a linked list of labelNode
    labelNode *lnX = getSetNode(head,setX)->labels;
    labelNode *yLabelNode = NULL;
    int xLabel = -1;

    bool exists;
    while (lnX != NULL) {
        // get head of setY labels
        yLabelNode = getSetNode(head, setY)->labels;
        xLabel = lnX->label;
        exists = false;
        // iterate through all of setY labels
        while (yLabelNode != NULL && !exists) {
            if (yLabelNode->label == xLabel) {
                exists= true;
            }
            yLabelNode=yLabelNode->next;
        }
        if (!exists) { // if X label not found in Y, add the label to Y
            pushLabelNode(&getSetNode(head, setY)->labels, xLabel);
        }
        lnX = lnX->next;
    }
}

void unionEquivalenceLabels(setNode* head, int X, int Y)
{
    if (X == Y) return;
    combineSetIDLabels(head, Y, X);
    combineSetIDLabels(head, X, Y);
}

void pushSetID(setNode** head, int ID)
{
    setNode *tempSN = (setNode *)malloc(sizeof(setNode));
    initializeSetNode(tempSN);
    tempSN->setID = ID;

    // necessary for unionEquivalenceLabels
    labelNode *ln = (labelNode *)malloc(sizeof(labelNode));
    initializeLabelNode(ln);
    ln->label = ID;
    tempSN->labels = ln;

    tempSN->next = (*head);
    (*head) = tempSN;
}

void listSetNodesLabels(setNode* head, int setNode)
{
    labelNode *lnhead = getSetNode(head, setNode)->labels;
    printf("SetNode %d: ", setNode);
    while (lnhead != NULL) {
        printf("%d", lnhead->label);
        if (lnhead->next != NULL) printf(", ");
        lnhead=lnhead->next;
    }
    printf("\n");
}


void listEquivalencetable(setNode* head)
{
    setNode *tmp = head;
    while (tmp != NULL) {
        listSetNodesLabels(head, tmp->setID);
        tmp = tmp->next;
    }
}

