/*
 * local_realignment.h
 *
 *  Created on: Sep 3, 2014
 *      Author: Ivan Sovic
 */

#ifndef MYERS_WRAPPER_H_
#define MYERS_WRAPPER_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string>
#include <sstream>
#include <vector>

#include "index/index_sa.h"
#include "sequences/single_sequence.h"
#include "utility/utility_general.h"
#include "utility/program_parameters.h"
#include "utility/utility_conversion-inl.h"
#include "containers/path_graph_entry.h"
#include "alignment/myers.h"
#include "alignment/cigargen.h"
#include "log_system/log_system.h"
#include "containers/region.h"
#include "seqan/basic.h"
#include "seqan/align.h"
#include "seqan/sequence.h"
#include "seqan/stream.h"

#include "alignment/local_realignment_wrappers.h"



typedef int (*AlignmentFunctionType)(const int8_t*, int64_t, const int8_t*, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t*, int64_t*, int64_t*, std::vector<unsigned char> &);
typedef int (*EditDistanceFunctionType)(const int8_t*, int64_t, const int8_t*, int64_t, int64_t*, int64_t*, int);

typedef int (*AlignmentFunctionTypeMex)(const int8_t*, int64_t, const int8_t*, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t*, int64_t*, int64_t*, std::vector<unsigned char> &);

int ClipCircularAlignment(int64_t alignment_start, int64_t alignment_end, unsigned char *alignment, int64_t alignment_length,
                          int64_t read_length, int64_t reference_start, int64_t reference_length, int64_t split_region_start_offset, int64_t position_of_ref_end,
                          unsigned char **ret_left_alignment, int64_t *ret_left_alignment_length,
                          int64_t *ret_left_alignment_pos_start, int64_t *ret_left_alignment_pos_end,
                          unsigned char **ret_right_alignment, int64_t *ret_right_alignment_length,
                          int64_t *ret_right_alignment_pos_start, int64_t *ret_right_alignment_pos_end);

int LocalRealignmentLinear(AlignmentFunctionType AlignmentFunction, const SingleSequence *read, const Index *index, const ProgramParameters &parameters, const PathGraphEntry *best_path,
                           int64_t *ret_alignment_position_left_part, std::string *ret_cigar_left_part, int64_t *ret_AS_left_part, int64_t *ret_nonclipped_left_part,
                           int64_t *ret_alignment_position_right_part, std::string *ret_cigar_right_part, int64_t *ret_AS_right_part, int64_t *ret_nonclipped_right_part,
                           SeqOrientation *ret_orientation, int64_t *ret_reference_id, int64_t *ret_position_ambiguity,
                           int64_t *ret_eq_op, int64_t *ret_x_op, int64_t *ret_i_op, int64_t *ret_d_op, bool perform_reverse_complement=true);

int LocalRealignmentCircular(AlignmentFunctionType AlignmentFunction, const SingleSequence *read, const Index *index, const ProgramParameters &parameters, const PathGraphEntry *best_path,
                             int64_t *ret_alignment_position_left_part, std::string *ret_cigar_left_part, int64_t *ret_AS_left_part, int64_t *ret_nonclipped_left_part,
                             int64_t *ret_alignment_position_right_part, std::string *ret_cigar_right_part, int64_t *ret_AS_right_part, int64_t *ret_nonclipped_right_part,
                             SeqOrientation *ret_orientation, int64_t *ret_reference_id, int64_t *ret_position_ambiguity,
                             int64_t *ret_eq_op, int64_t *ret_x_op, int64_t *ret_i_op, int64_t *ret_d_op, bool perform_reverse_complement=true);
int HybridRealignment(const SingleSequence *read, const Index *index, const ProgramParameters &parameters, const PathGraphEntry *best_path,
                      int64_t *ret_alignment_position_left_part, std::string *ret_cigar_left_part, int64_t *ret_AS_left_part, int64_t *ret_nonclipped_left_part,
                      int64_t *ret_alignment_position_right_part, std::string *ret_cigar_right_part, int64_t *ret_AS_right_part, int64_t *ret_nonclipped_right_part,
                      SeqOrientation *ret_orientation, int64_t *ret_reference_id, int64_t *ret_position_ambiguity,
                      int64_t *ret_eq_op, int64_t *ret_x_op, int64_t *ret_i_op, int64_t *ret_d_op, bool perform_reverse_complement=true);

int AnchoredAlignmentLinear(AlignmentFunctionType AlignmentFunctionNW, AlignmentFunctionType AlignmentFunctionSemiglobal, const SingleSequence *read, const Index *index, const ProgramParameters &parameters, const PathGraphEntry *best_path,
                           int64_t *ret_alignment_position_left_part, std::string *ret_cigar_left_part, int64_t *ret_AS_left_part, int64_t *ret_nonclipped_left_part,
                           int64_t *ret_alignment_position_right_part, std::string *ret_cigar_right_part, int64_t *ret_AS_right_part, int64_t *ret_nonclipped_right_part,
                           SeqOrientation *ret_orientation, int64_t *ret_reference_id, int64_t *ret_position_ambiguity,
                           int64_t *ret_eq_op, int64_t *ret_x_op, int64_t *ret_i_op, int64_t *ret_d_op, bool perform_reverse_complement=true);

int AnchoredAlignment(bool is_linear, bool end_to_end, AlignmentFunctionType AlignmentFunctionNW, AlignmentFunctionType AlignmentFunctionSHW, const SingleSequence *read, const Index *index, const ProgramParameters &parameters, const PathGraphEntry *best_path,
                           int64_t *ret_alignment_position_left_part, std::string *ret_cigar_left_part, int64_t *ret_AS_left_part, int64_t *ret_nonclipped_left_part,
                           int64_t *ret_alignment_position_right_part, std::string *ret_cigar_right_part, int64_t *ret_AS_right_part, int64_t *ret_nonclipped_right_part,
                           SeqOrientation *ret_orientation, int64_t *ret_reference_id, int64_t *ret_position_ambiguity,
                           int64_t *ret_eq_op, int64_t *ret_x_op, int64_t *ret_i_op, int64_t *ret_d_op, bool perform_reverse_complement=true);

int AnchoredAlignmentMex(bool is_linear, bool end_to_end, AlignmentFunctionTypeMex AlignmentFunctionNW, AlignmentFunctionTypeMex AlignmentFunctionSHW, const SingleSequence *read, const Index *index, const ProgramParameters &parameters, const PathGraphEntry *best_path,
                           int64_t *ret_alignment_position_left_part, std::string *ret_cigar_left_part, int64_t *ret_AS_left_part, int64_t *ret_nonclipped_left_part,
                           int64_t *ret_alignment_position_right_part, std::string *ret_cigar_right_part, int64_t *ret_AS_right_part, int64_t *ret_nonclipped_right_part,
                           SeqOrientation *ret_orientation, int64_t *ret_reference_id, int64_t *ret_position_ambiguity,
                           int64_t *ret_eq_op, int64_t *ret_x_op, int64_t *ret_i_op, int64_t *ret_d_op, bool perform_reverse_complement=true);

int CalcEditDistanceLinear(EditDistanceFunctionType EditDistanceFunction, const SingleSequence *read, const Index *index, const ProgramParameters &parameters, const PathGraphEntry *best_path,
                           int64_t *ret_alignment_position, int64_t *ret_edit_distance);
int CalcEditDistanceCircular(EditDistanceFunctionType EditDistanceFunction, const SingleSequence *read, const Index *index, const ProgramParameters &parameters, const PathGraphEntry *best_path,
                             int64_t *ret_alignment_position, int64_t *ret_edit_distance);



int CheckAlignmentSane(std::vector<unsigned char> &alignment, const SingleSequence* read=NULL, const Index* index=NULL, int64_t reference_hit_id=-1, int64_t reference_hit_pos=-1);
int CountAlignmentOperations(std::vector<unsigned char> &alignment, const int8_t *read_data, const int8_t *ref_data, int64_t reference_hit_id, int64_t alignment_position_start, SeqOrientation orientation,
                             int64_t match, int64_t mismatch, int64_t gap_open, int64_t gap_extend,
                             int64_t *ret_eq, int64_t *ret_x, int64_t *ret_i, int64_t *ret_d, int64_t *ret_alignment_score, int64_t *ret_nonclipped_length);

#endif /* MYERS_WRAPPER_H_ */
