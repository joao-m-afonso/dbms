/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
#include "operators.hh"
#include "block.hh"

namespace engine {

enum type {
  filtered_bit_vector = 0x1,  // fbv: --c
  bitmap = 0x2,  // bm: -r-
  filtered_bitmap = 0x3,  // fbm: -rc
  decimal_vector = 0x04,  // dv: v--
  filtered_decimal_vector = 0x5,  // fdv: v-c
  decimal_map = 0x6,  // dm: vr-
  filtered_decimal_map = 0x7,  // fdm: vrc
}

// 0x11 (fbv-fbv) Khatri-Rao of two filtered bit vectors
void
CSC_krao_fbv_fbv(block* A, block* B, block* C) {
}

void
COO_krao_fbv_fbv(block* A, block* B, block* C) {
  int i = 0, j = 0;
  while (i < A->columns.size() && j < B->columns.size()) {
    if ( A->columns[i] < B->columns[j] )
      ++i;
    else if ( A->columns[i] > B->columns[j] )
      ++j;
    else
      C->columns.push_back(A->columns[i]);
  }
}


// 0x12 (fbv-bm) Khatri-Rao of a filtered bit vector with a bitmap
// 0x21 (bm-fbv)
void
move_COO_krao_fbv_bm(block* A, block* B, block* C) {
  C->rows.reserve(A->columns.size());
  for (auto& col : A->columns) {
    C->rows.push_back(B->rows[col]);
  }
  C->columns = std::move(A->columns);
}

void
CSC_krao_fbv_bm(block* A, block* B, block* C) {
}

void
COO_krao_fbv_bm(block* A, block* B, block* C) {
  C->rows.reserve(A->columns.size());
  for (auto& col : A->columns) {
    C->rows.push_back(B->rows[col]);
  }
  C->columns = A->columns;
}


// 0x13 (fbv-fbm) Khatri-Rao product of a filtered bit vector
// 0x31 (fbm-fbv) with a filtered bitmap
void
CSC_krao_fbv_fbm(block* A, block* B, block* C) {
}

void
COO_krao_fbv_fbm(block* A, block* B, block* C) {
  int i = 0, j = 0;
  while (i < A->columns.size() && j < B->columns.size()) {
    if ( A->columns[i] < B->columns[j] ) {
      ++i;
    } else if ( A->columns[i] > B->columns[j] ) {
      ++j;
    } else {
      C->columns.push_back(A->columns[i]);
      C->rows.push_back(B->rows[j]);
    }
  }
}


// 0x14 (fbv-dv) Khatri-Rao product of a filtered bit vector
// 0x41 (dv-fdv) with a decimal vector
void
move_COO_krao_fbv_dv(block* A, block* B, block* C) {
  C->values.reserve(A->columns.size());
  for (auto &col : A->columns) {
    C->values.push_back(B->values[col]);
  }
  C->columns = std::move(A->columns);
}

void
CSC_krao_fbv_dv(block* A, block* B, block* C) {
}

void
COO_krao_fbv_dv(block* A, block* B, block* C) {
  C->values.reserve(A->columns.size());
  for (auto& col : A->columns) {
    C->values.push_back(B->values[col]);
  }
  C->columns = A->columns;
}


// 0x15 (fbv-fdv)
// 0x51 (fbv-fdv)
void
CSC_krao_fbv_fdv(block* A, block* B, block* C) {
}

void
COO_krao_fbv_fdv(block* A, block* B, block* C) {
  int i = 0, j = 0;
  while (i < A->columns.size() && j < B->columns.size()) {
    if ( A->columns[i] < B->columns[j] ) {
      ++i;
    } else if ( A->columns[i] > B->columns[j] ) {
      ++j;
    } else {
      C->columns.push_back(A->columns[i]);
      C->values.push_back(B->values[j]);
    }
  }
}


// 0x16 (fbv-dm)
// 0x61 (dm-fdv)
void
move_COO_krao_fbv_dm(block* A, block* B, block* C) {
  C->values.reserve(A->columns.size());
  C->rows.reserve(A->columns.size());
  for (auto &col : A->columns) {
    C->values.push_back(B->values[col]);
    C->rows.push_back(B->rows[col]);
  }
  C->columns = std::move(A->columns);
}

void
CSC_krao_fbv_dm(block* A, block* B, block* C) {
}

void
COO_krao_fbv_dm(block* A, block* B, block* C) {
  C->values.reserve(A->columns.size());
  C->rows.reserve(A->columns.size());
  for (auto& col : A->columns) {
    C->values.push_back(B->values[col]);
    C->rows.push_back(B->rows[col]);
  }
  C->columns = A->columns;
}


// 0x17 (fbv-fbm) Khatri-Rao product of a filtered bit vector
// 0x71 (fbm-fbv) with a filtered decimal map
void
CSC_krao_fbv_fdm(block* A, block* B, block* C) {
}

void
COO_krao_fbv_fdm(block* A, block* B, block* C) {
  int i = 0, j = 0;
  while (i < A->columns.size() && j < B->columns.size()) {
    if ( A->columns[i] < B->columns[j] ) {
      ++i;
    } else if ( A->columns[i] > B->columns[j] ) {
      ++j;
    } else {
      C->columns.push_back(A->columns[i]);
      C->rows.push_back(B->rows[j]);
      C->values.push_back(B->values[j]);
    }
  }
}


// 0x22 (bm-bm)
void
move_krao_bm_bm(block* A, block* B, block* C, int rows_A) {
  #ifdef DEBUG
    assert(A->rows.size() == B->rows.size());
  #endif  // DEBUG
  for (int i = 0; i < A->rows.size(); ++i) {
    A->rows[i] = A->rows[i] * rows_A + B->rows[i];
  }
  C->rows = std::move(A->rows);
}

void
krao_bm_bm(block* A, block* B, block* C, int rows_A) {
  #ifdef DEBUG
    assert(A->rows.size() == B->rows.size());
  #endif  // DEBUG
  C->rows.reserve(A->rows.size());
  for (int i = 0; i < A->rows.size(); ++i) {
    C->rows.push_back(A->rows[i] * rows_A + B->rows[i]);
  }
}


// 0x23 (bm-fbm)
void
move_COO_krao_bm_fbm(block* A, block* B, block* C, int rows_A) {
  C->rows.reserve(B->columns.size());
  for (int i = 0; i < B->columns.size(); ++i) {
    int col = B->columns[i];
    C->rows.push_back(A->rows[col] * rows_A + B->rows[i]);
  }
  C->columns = std::move(B->columns);
}

void
CSC_krao_bm_fbm(block* A, block* B, block* C, int rows_A) {
}

void
COO_krao_bm_fbm(block* A, block* B, block* C, int rows_A) {
  C->rows.reserve(B->columns.size());
  for (int i = 0; i < B->columns.size(); ++i) {
    int col = B->columns[i];
    C->rows.push_back(A->rows[col] * rows_A + B->rows[i]);
  }
  C->columns = B->columns;
}


// 0x24 (bm-dv) Khatri-Rao product of a bitmap with a decimal vector
// 0x42 (dv-bm)
void
move_krao_bm_dv(block* A, block* B, block* C) {
  #ifdef DEBUG
    assert(A->rows.size() == B->values.size());
  #endif  // DEBUG
  C->rows = std::move(A->rows);
  C->values = std::move(B->values);
}

void
krao_bm_dv(block* A, block* B, block* C) {
  #ifdef DEBUG
    assert(A->rows.size() == B->values.size());
  #endif  // DEBUG
  C->rows = A->rows;
  C->values = B->values;
}


// 0x25 (bm-fdv)
// 0x52 (fdv-bm)
void
move_CSC_krao_bm_fdv(block* A, block* B, block* C) {
}

void
move_COO_krao_bm_fdv(block* A, block* B, block* C) {
  move_COO_krao_fbv_bm(B, A, C);
  C->values = std::move(B->values);
}

void
CSC_krao_bm_fdv(block* A, block* B, block* C) {
}

void
COO_krao_bm_fdv(block* A, block* B, block* C) {
  COO_krao_fbv_bm(B, A, C);
  C->values = B->values;
}


// 0x26 (bm-dm)
void
move_krao_bm_dm(block* A, block* B, block* C, int rows_A) {
  move_krao_bm_bm(A, B, C, rows_A);
  C->values = std::move(B->values);
}

void
krao_bm_dm(block* A, block* B, block* C, int rows_A) {
  krao_bm_bm(A, B, C, rows_A);
  C->values = B->values;
}


// 0x27 (bm-fdm)
void
move_CSC_krao_bm_fdm(block* A, block* B, block* C, int rows_A) {
}

void
move_COO_krao_bm_fdm(block* A, block* B, block* C, int rows_A) {
  move_COO_krao_bm_fbm(A, B, C, rows_A);
  C->values = std::move(B->values);
}

void
CSC_krao_bm_fdm(block* A, block* B, block* C, int rows_A) {
}

void
COO_krao_bm_fdm(block* A, block* B, block* C, int rows_A) {
  COO_krao_bm_fbm(A, B, C, rows_A);
  C->values = B->values;
}


// 0x32 (fbm-bm)
void
move_COO_krao_fbm_bm(block* A, block* B, block* C, int rows_A) {
  C->rows.reserve(A->columns.size());
  for (int i = 0; i < A->columns.size(); ++i) {
    int col = A->columns[i];
    C->rows.push_back(A->rows[i] * rows_A + B->rows[col]);
  }
  C->columns = std::move(A->columns);
}

void
CSC_krao_fbm_bm(block* A, block* B, block* C, int rows_A) {
}

void
COO_krao_bm_fbm(block* A, block* B, block* C, int rows_A) {
  C->rows.reserve(A->columns.size());
  for (int i = 0; i < A->columns.size(); ++i) {
    int col = A->columns[i];
    C->rows.push_back(A->rows[i] * rows_A + B->rows[col]);
  }
  C->columns = A->columns;
}


// 0x33 (fbm-fbm)
void
CSC_krao_fbm_fbm(block* A, block* B, block* C, int rows_A) {
}

void
COO_krao_fbm_fbm(block* A, block* B, block* C, int rows_A) {
  int i = 0, j = 0;
  while (i < A->columns.size() && j < B->columns.size()) {
    if ( A->columns[i] < B->columns[j] ) {
      ++i;
    } else if ( A->columns[i] > B->columns[j] ) {
      ++j;
    } else {
      C->columns.push_back(A->columns[i]);
      C->rows.push_back(A->rows[i] * rows_A + B->rows[j]);
    }
  }
}


// 0x34 (fbm-dv)
// 0x43 (dv-fbm)
void
move_CSC_krao_fbm_dv(block* A, block* B, block* C) {
}

void
move_COO_krao_fbm_dv(block* A, block* B, block* C) {
  move_COO_krao_fbv_dv(B, A, C);
  C->rows = std::move(A->rows);
}

void
CSC_krao_fbm_dv(block* A, block* B, block* C) {
}

void
COO_krao_fbm_dv(block* A, block* B, block* C) {
  COO_krao_fbv_dv(B, A, C);
  C->rows = B->rows;
}


// 0x35 (fbm-fdv)
// 0x53 (fdv-fbm)


// 0x36 (fbm-dm)


// 0x37 (fbm-fdm)


// 0x44 (dv-dv) Khatri-Rao product of two decimal vectors
void
move_krao_dv_dv(block* A, block* B, block* C) {
  #ifdef DEBUG
    assert(A->values.size() == B->values.size());
  #endif
  for (int i = 0; i < A->values.size(); ++i) {
    A->values[i] *= B->values[i];
  }
  C->values = std::move(A->values);
}

void
krao_dv_dv(block* A, block* B, block* C) {
  #ifdef DEBUG
    assert(A->values.size() == B->values.size());
  #endif
  C->values.reserve(A->values.size());
  for (int i = 0; i < A->values.size(); ++i) {
    C->values.push_back(A->values[i] * B->values[i]);
  }
}


// 0x45 (dv-fdv)
// 0x54 (fdv-dv)


// 0x46 (dv-dm)
// 0x64 (dm-dv)


// 0x47 (dv-fdm)
// 0x74 (fdm-dv)


// 0x55 (fdv-fdv)


// 0x56 (fdv-dm)
// 0x65 (dm-fdv)


// 0x57 (fdv-fdm)
// 0x75 (fdm-fdv)


// 0x62 (dm-bm)
void
move_krao_dm_bm(block* A, block* B, block* C) {
  move_krao_bm_bm(A, B, C);
  C->values = std::move(A->values);
}

void
krao_dm_bm(block* A, block* B, block* C) {
  krao_bm_bm(A, B, C);
  C->values = A->values;
}

// 0x63 (dm-fbm)


// 0x66 (dm-dm)


// 0x67 (dm-fdm)


// 0x72 (fdm-bm)
void
move_CSC_krao_fdm_bm(block* A, block* B, block* C, int rows_A) {
}

void
move_COO_krao_fdm_bm(block* A, block* B, block* C, int rows_A) {
  move_COO_krao_fbm_bm(A, B, C, rows_A);
  C->values = std::move(A->values);
}

void
CSC_krao_fdm_bm(block* A, block* B, block* C, int rows_A) {
}

void
COO_krao_fdm_bm(block* A, block* B, block* C, int rows_A) {
  COO_krao_fbm_bm(A, B, C, rows_A);
  C->values = A->values;
}


// 0x73 (fdm-fbm)


// 0x76 (fdm-dm)


// 0x77 (fdm-fdm)



void
krao(block* A,
     block* B,
     block* C,
     int rows_A,
     bool CSC = false,
     bool move = false) {
  #ifdef DEBUG
    assert(A->index == B->index);
  #endif  // DEBUG
  C->index = A->index;

  char hex = (A->values.size() >0) << 6 |
             (A->rows.size() > 0) << 5 |
             (A->columns.size() > 0) << 4 |
             (B->values.size() > 0) << 2 |
             (B->rows.size() > 0) << 1 |
             (B->columns.size() > 0);

  switch (hex) {
    case 0x11:
      if (CSC)
        CSC_krao_fbv_fbv(A, B, C);
      else
        COO_krao_fbv_fbv(A, B, C);
      break;

    case 0x12:
      if (CSC)
        CSC_krao_fbv_bm(A, B, C);
      else if (move)
        move_COO_krao_fbv_bm(A, B, C);
      else
        COO_krao_fbv_bm(A, B, C);
      break;

    case 0x13:
      if (CSC)
        CSC_krao_fbv_fbm(A, B, C);
      else
        COO_krao_fbv_fbm(A, B, C);
      break;

    case 0x14:
      if (CSC)
        CSC_krao_fbv_dv(A, B, C);
      else if (move)
        move_COO_krao_fbv_dv(A, B, C);
      else
        COO_krao_fbv_dv(A, B, C);
      break;

    case 0x15:
      if (CSC)
        CSC_krao_fbv_fdv(A, B, C);
      else
        COO_krao_fbv_fdv(A, B, C);
      break;

    case 0x16:
      if (CSC)
        CSC_krao_fbv_dm(A, B, C);
      else if (move)
        move_COO_krao_fbv_dm(A, B, C);
      else
        COO_krao_fbv_dm(A, B, C);
      break;

    case 0x17:
      if (CSC)
        CSC_krao_fbv_fdm(A, B, C);
      else
        COO_krao_fbv_fdm(A, B, C);
      break;

    case 0x21:
      if (move && CSC)
        move_CSC_krao_fbv_bm(B, A, C);
      else if (move)
        move_COO_krao_fbv_bm(B, A, C);
      else if (CSC)
        CSC_krao_fbv_bm(B, A, C);
      else
        COO_krao_fbv_bm(B, A, C);
      break;

    case 0x22:
      if (move)
        move_krao_bm_bm(A, B, C, rows_A);
      else
        krao_bm_bm(A, B, C, rows_A);
      break;

    case 0x23:
      if (CSC)
        CSC_krao_bm_fbm(A, B, C, rows_A);
      else if (move)
        move_COO_krao_bm_fbm(A, B, C, rows_A);
      else
        COO_krao_bm_fbm(A, B, C, rows_A);
      break;

    case 0x24:
      if (move)
        move_krao_bm_dv(A, B, C);
      else
        krao_bm_dv(A, B, C);
      break;

    case 0x25:
      if (move && CSC)
        move_CSC_krao_bm_fdv(A, B, C);
      else if (move)
        move_COO_krao_bm_fdv(A, B, C);
      else if (CSC)
        CSC_krao_bm_fdv(A, B, C);
      else
        COO_krao_bm_fdv(A, B, C);
      break;

    case 0x26:
      if (move)
        move_krao_bm_dm(A, B, C, rows_A);
      else
        krao_bm_dm(A, B, C, rows_A);
      break;

    case 0x27:
      if (move && CSC)
        move_CSC_krao_bm_fdm(A, B, C, rows_A);
      else if (move)
        move_COO_krao_bm_fdm(A, B, C, rows_A);
      else if (CSC)
        CSC_krao_bm_fdm(A, B, C, rows_A);
      else
        COO_krao_bm_fdm(A, B, C, rows_A);
      break;

    case 0x31:
      if (CSC)
        CSC_krao_fbv_fbm(B, A, C);
      else
        COO_krao_fbv_fbm(B, A, C);
      break;

    case 0x32:
      if (CSC)
        CSC_krao_fbm_bm(A, B, C, rows_A);
      else if (move)
        move_COO_krao_fbm_bm(A, B, C, rows_A);
      else
        COO_krao_fbm_bm(A, B, C, rows_A);
      break;

    case 0x33:
      if (CSC)
        CSC_krao_fbm_fbm(A, B, C, rows_A);
      else
        COO_krao_fbm_fbm(A, B, C, rows_A);
      break;

    case 0x34:
      if (move && CSC)
        move_CSC_krao_fbm_dv(A, B, C);
      else if (move)
        move_COO_krao_fbm_dv(A, B, C);
      else if (CSC)
        CSC_krao_fbm_dv(A, B, C);
      else
        COO_krao_fbm_dv(A, B, C);
      break;





    case 0x44:
      if (move)
        move_krao_dv_dv(A, B, C);
      else
        krao_dv_dv(A, B, C);
      break;

    default:
      break;
  }  // switch (hex)
}  // krao()
}  // namespace engine