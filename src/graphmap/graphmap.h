/*
 * treemap_se.h
 *
 *  Created on: Jul 19, 2014
 *      Author: ivan
 */

#ifndef TREEMAP_SE_H_
#define TREEMAP_SE_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "index/index.h"
#include "index/index_sa.h"
//#include "index/index_spaced_sa.h"
#include "index/index_hash.h"
#include "index/index_spaced_hash.h"
#include "index/index_spaced_hash_fast.h"
#include "sequences/single_sequence.h"
#include "sequences/sequence_file.h"
//#include "containers/vertex_data.h"
#include "containers/score_registry.h"
#include "utility/program_parameters.h"
#include "utility/utility_general.h"
#include "alignment/cigargen.h"
#include "alignment/local_realignment.h"
#include "containers/region.h"
//#include "graphmap/path_graph_registry.h"
//#include "graphmap/alignment_object.h"
#include "containers/mapping_data.h"
#include "utility/evalue.h"
#include "containers/vertices.h"



class GraphMap {
 public:
  GraphMap();
  ~GraphMap();

  // Main function for running the mapping process. It generates/loads the index, and handles batch loading of sequences from the reads file.
  void Run(ProgramParameters &parameters);
  // Generates or loads the index of the reference genome.
  int BuildIndex(ProgramParameters &parameters);
  // Loads reads from a file in batches of given size (in MiB), or all at once.
  void ProcessReadsFromSingleFile(ProgramParameters &parameters, FILE *fp_out);
  // Process the loaded batch of reads. Uses OpenMP to do it in parallel. Calls ProcessOneRead for each read in the SequenceFile.
  int ProcessSequenceFileInParallel(ProgramParameters *parameters, SequenceFile *reads, clock_t *last_time, FILE *fp_out, int64_t *ret_num_mapped, int64_t *ret_num_unmapped);

  int ProcessRead(MappingData *mapping_data, const Index *index, const Index *index_secondary, const SingleSequence *read, const ProgramParameters *parameters, const EValueParams *evalue_params);
  int CollectSAMLines(std::string &ret_sam_lines, MappingData *mapping_data, const SingleSequence *read, const ProgramParameters *parameters);

  int CollectAMOSLines(std::string &ret_amos_lines, MappingData *mapping_data, const SingleSequence *read, const ProgramParameters *parameters);



 private:
  SequenceFile *reference_;

  Index *index_;
  Index *index_secondary_;

  // Retrieves a file list from the given folder.
  bool GetFileList_(std::string folder, std::vector<std::string> &ret_files);
  // Check if string ends with the given suffix (parameter 'ending'), and returns true if so.
  bool StringEndsWith_(std::string const &full_string, std::string const &ending);
  // Returns only files with one of the following extensions: fasta, fastq, fa, fq, sam.
  void FilterFileList_(std::vector<std::string> &files, std::vector<std::string> &ret_read_files, std::vector<std::string> &ret_sam_files);
  // Opens the output SAM file for writing if the path is specified. If the path is empty, then output is set to STDOUT.
  FILE* OpenOutSAMFile_(std::string out_sam_path="");
  // Formats and writes the SAM header from a given index.
//  int WriteSAMHeader_(ProgramParameters &parameters, Index *index, FILE *fp_out);
  std::string GenerateSAMHeader_(ProgramParameters &parameters, Index *index);
  std::string GenerateUnmappedSamLine_(MappingData *mapping_data, int64_t verbose_sam_output, const SingleSequence *read) const;

  // Calculates the LCSk of the anchors using the Fenwick tree.
  void CalcLCSFromLocalScoresCacheFriendly_(const Vertices *vertices, bool use_l1_filtering, int64_t l, int64_t allowed_dist, int* ret_lcskpp_length, std::vector<int> *ret_lcskpp_indices);

  int RegionSelectionSpacedHashFast_(int64_t bin_size, MappingData *mapping_data, const Index *index, const Index *index_secondary, const SingleSequence *read, const ProgramParameters *parameters);
  int RegionSelectionSpacedHash_(int64_t bin_size, MappingData *mapping_data, const IndexSpacedHash *index, const IndexSpacedHash *index_secondary, const SingleSequence *read, const ProgramParameters *parameters);
  int RegionSelectionSpacedHashv2_(int64_t bin_size, MappingData *mapping_data, const IndexSpacedHash *index_primary, const IndexSpacedHash *index_secondary, const SingleSequence *read, const ProgramParameters *parameters);
  int ExperimentalRegionSelection_(int64_t bin_size, MappingData *mapping_data, const Index *index, const Index *index_secondary, const SingleSequence *read, const ProgramParameters *parameters);
  int RegionSelectionSpacedHashFastv2_(int64_t bin_size, MappingData *mapping_data, const IndexSpacedHashFast *index_primary, const IndexSpacedHashFast *index_secondary, const SingleSequence *read, const ProgramParameters *parameters);

  int GraphMap_(ScoreRegistry *local_score, Index *index_read, MappingData *mapping_data, const Index *index, const Index *index_secondary, const SingleSequence *read, const ProgramParameters *parameters);
  int ProcessKmerCacheFriendly_(int8_t *kmer, int64_t kmer_start_position, ScoreRegistry *local_score, MappingData* mapping_data, Index *index_read, const Index* index, const Index* index_secondary, const SingleSequence* read, const ProgramParameters* parameters);
  int PostProcessRegionWithLCS_(ScoreRegistry *local_score, MappingData *mapping_data, const Index *index, const Index *index_secondary, const SingleSequence *read, const ProgramParameters *parameters);
  int CalculateL1ParametersWithMaximumDeviation_(ScoreRegistry *local_score, std::vector<int> &lcskpp_indices, float maximum_allowed_deviation, int64_t *ret_k, int64_t *ret_l, float *ret_sigma_L2, float *ret_confidence_L1);
  int64_t CountBinsWithinThreshold_(const MappingData *mapping_data, float threshold);
  Region CalcRegionFromBin_(int64_t sorted_bins_index, const MappingData *mapping_data, const SingleSequence *read, const ProgramParameters *parameters);
  int CheckRegionSearchFinished_(int64_t current_region, float min_allowed_bin_value, float threshold_step, float *bin_value_threshold, MappingData *mapping_data, const SingleSequence *read, const ProgramParameters *parameters);
  int EvaluateMappings_(bool evaluate_edit_distance, MappingData *mapping_data, const SingleSequence *read, const ProgramParameters *parameters);
  int GenerateAlignments_(MappingData *mapping_data, const Index *index, const SingleSequence *read, const ProgramParameters *parameters, const EValueParams *evalue_params);
  int CollectFinalMappingsAndMapQ_(bool generate_final_mapping_ptrs, MappingData *mapping_data, const SingleSequence *read, const ProgramParameters *parameters);
  int CheckMinimumMappingConditions_(InfoMapping *mapping_data, InfoL1 *l1_data, const Index *index, const SingleSequence *read, const ProgramParameters *parameters);
  int VerboseLocalScoresToFile(std::string file_path, const SingleSequence *read, const ScoreRegistry *local_score, const std::vector<int> *indices, int64_t l_median, float maximum_allowed_deviation, bool check_median_filtering, std::vector<int32_t> *cluster_ids=NULL);

  int AnchoredPostProcessRegionWithLCS_(ScoreRegistry *local_score, MappingData *mapping_data, const Index *index, const Index *index_secondary, const SingleSequence *read, const ProgramParameters *parameters);
  int ExperimentalPostProcessRegionWithLCS1_(ScoreRegistry *local_score, MappingData *mapping_data, const Index *index, const Index *index_secondary, const SingleSequence *read, const ProgramParameters *parameters);
//  int FilterAnchorBreakpoints(const std::vector<int> &lcskpp_indices, int64_t ref_hits_start, int64_t ref_hits_end, int64_t seed_length, int64_t min_cluster_length, float min_cluster_coverage, OwlerData* owler_data, std::vector<Index*> &indexes, const SingleSequence* read, const ProgramParameters* parameters, std::vector<int> &ret_filtered_lcskpp_indices, std::vector<int32_t> *ret_cluster_ids=NULL);
  int LCSForAnchoredAlignment_(ScoreRegistry *local_score, MappingData *mapping_data, const Index *index, const Index *index_secondary, const SingleSequence *read, const ProgramParameters *parameters);
};

#endif /* TREEMAP_SE_H_ */
