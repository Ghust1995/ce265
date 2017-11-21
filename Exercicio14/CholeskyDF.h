//Driver para Cholesky paralelo heroico

void CholeskyDF(ptrBM A, double *timeDF);

// Cholesky paralelo heorico por blocos
// decompoe MatrizSimetrica(A) em L e L^t 
// retorna L na triangular inferior de A


void CholeskyParBM(ptrBM A, int nBK, int threadID, int nThreads, 
		   int volatile **core_progress);
