#Where are include files kept
HEADER_INDEX=../../../HeaderFiles/index/
HEADER_METRICDISTANCE=../../../HeaderFiles/metricdistance/
HEADER_METRICDATA=../../../HeaderFiles/metricdata/
HEADER_QUERY=../../../HeaderFiles/index/query/
HEADER_UTIL=../../../HeaderFiles/util/
HEADER_GSL=../../../SourceFiles/util/GSL/include/gsl/
HEADER_CLUSTER=../../../HeaderFiles/cluster/
HEADER_CLUSTER_UTIL=../../../HeaderFiles/cluster/util/
HEADER_CLASSIFIER=../../../HeaderFiles/classifier/
HEADER_CLASSIFIER_UTIL=../../../HeaderFiles/classifier/util/
SOURCE_INDEX=../../../SourceFiles/index/
SOURCE_METRICDISTANCE=../../../SourceFiles/metricdistance/
SOURCE_METRICDATA=../../../SourceFiles/metricdata/
SOURCE_QUERY=../../../SourceFiles/index/query/
SOURCE_UTIL=../../../SourceFiles/util/
SOURCE_CLUSTER=../../../SourceFiles/cluster/
SOURCE_CLUSTER_UTIL=../../../SourceFiles/cluster/util/
SOURCE_CLASSIFIER=../../../SourceFiles/classifier/
SOURCE_CLASSIFIER_UTIL=../../../SourceFiles/classifier/util/

#Options for development
CFLAGS=-g -Wall -std=c++0x -lpthread

UMAD: BuildORQueryIndex.o Index.o IndexNode.o ListIndex.o MVPIndex.o MVPInternalNode.o MVPLeafNode.o BalancePartitionMethod.o ExcludedMiddlePartitionMethod.o FFTPivotSelectionMethod.o IncrementalSelection.o PartitionResults.o PCA.o PCAOnFFT.o CountedMetric.o DNAMetric.o EditDistance.o EuclideanDistance.o ImageMetric.o MetricDistance.o PeptideMetric.o RNAMetric.o DNA.o DoubleVector.o Image.o MetricData.o Peptide.o RNA.o StringObject.o Query.o RangeQuery.o SearchTask.o DoubleIndexObjectPair.o getopt.o LoadOrSearch.o MVPIndexStructureAnalysor.o ObjectFactory.o MemMonitor.o ExcludedMiddleForest.o TandemSpectra.o  main.o LInfinityDistance.o Clustering_Kmeans.o ClusteringTask.o utilFunction.o PartitionByOnePivot_BalanceOne.o PartitionByOnePivot_Balance.o PartitionByOnePivot_DBSCAN.o PartitionByOnePivot_Kmeans.o PartitionByOnePivot_Trisection.o PivotWisePartitionMethod.o SelectOptimalPivots_Sequence.o SelectOptimalPivots_Variance.o BuildAndSearchTypes.o MakeFileName.o ReadLabel.o DatasetInMetricSpace.o classification.o NavieBayes.o TrainOrTest.o Train.o Train_Knn.o Train_NavieBayes.o Train_C4.5.o Test.o Test_Knn.o Test_NavieBayes.o Test_C4.5.o ThreadPoolManager.o
	g++ -o UMAD BuildORQueryIndex.o Index.o IndexNode.o ListIndex.o MVPIndex.o MVPInternalNode.o MVPLeafNode.o BalancePartitionMethod.o ExcludedMiddlePartitionMethod.o FFTPivotSelectionMethod.o IncrementalSelection.o PartitionResults.o PCA.o PCAOnFFT.o CountedMetric.o DNAMetric.o EditDistance.o EuclideanDistance.o ImageMetric.o MetricDistance.o PeptideMetric.o RNAMetric.o DNA.o DoubleVector.o Image.o MetricData.o Peptide.o RNA.o StringObject.o Query.o RangeQuery.o SearchTask.o DoubleIndexObjectPair.o getopt.o LoadOrSearch.o MVPIndexStructureAnalysor.o ObjectFactory.o MemMonitor.o ExcludedMiddleForest.o TandemSpectra.o  main.o LInfinityDistance.o Clustering_Kmeans.o ClusteringTask.o utilFunction.o PartitionByOnePivot_BalanceOne.o PartitionByOnePivot_Balance.o PartitionByOnePivot_DBSCAN.o PartitionByOnePivot_Kmeans.o PartitionByOnePivot_Trisection.o PivotWisePartitionMethod.o SelectOptimalPivots_Sequence.o SelectOptimalPivots_Variance.o BuildAndSearchTypes.o MakeFileName.o ReadLabel.o DatasetInMetricSpace.o classification.o NavieBayes.o TrainOrTest.o Train.o Train_Knn.o Train_NavieBayes.o Train_C4.5.o Test.o Test_Knn.o Test_NavieBayes.o Test_C4.5.o ThreadPoolManager.o $(CFLAGS) `pkg-config --libs gsl`

main.o: $(SOURCE_UTIL)main.cpp 	
	g++ $(CFLAGS) -c $(SOURCE_UTIL)main.cpp
	
BuildORQueryIndex.o:
	g++ -c $(SOURCE_UTIL)BuildORQueryIndex.cpp $(CFLAGS)

Index.o: $(SOURCE_INDEX)Index.cpp $(HEADER_INDEX)Index.h
	g++ -I $(SOURCE_INDEX) $(CFLAGS) -c $(SOURCE_INDEX)Index.cpp

IndexNode.o: $(SOURCE_INDEX)IndexNode.cpp $(HEADER_INDEX)IndexNode.h
	g++ -I $(SOURCE_INDEX) $(CFLAGS) -c $(SOURCE_INDEX)IndexNode.cpp

ListIndex.o: $(SOURCE_INDEX)ListIndex.cpp $(HEADER_INDEX)ListIndex.h
	g++ -I $(SOURCE_INDEX) $(CFLAGS) -c $(SOURCE_INDEX)ListIndex.cpp

MVPIndex.o: $(SOURCE_INDEX)MVPIndex.cpp $(HEADER_INDEX)MVPIndex.h $(HEADER_METRICDATA)DoubleVector.h
	g++ -I $(SOURCE_INDEX) $(CFLAGS) -c $(SOURCE_INDEX)MVPIndex.cpp

MVPInternalNode.o: $(SOURCE_INDEX)MVPInternalNode.cpp $(HEADER_INDEX)MVPInternalNode.h
	g++ -I $(SOURCE_INDEX) $(CFLAGS) -c $(SOURCE_INDEX)MVPInternalNode.cpp

MVPLeafNode.o: $(SOURCE_INDEX)MVPLeafNode.cpp $(HEADER_INDEX)MVPLeafNode.h
	g++ -I $(SOURCE_INDEX) $(CFLAGS) -c $(SOURCE_INDEX)MVPLeafNode.cpp

BalancePartitionMethod.o: $(SOURCE_INDEX)BalancePartitionMethod.cpp $(HEADER_INDEX)BalancePartitionMethod.h $(HEADER_METRICDATA)DoubleIndexObjectPair.h $(HEADER_INDEX)MVPInternalNode.h $(HEADER_METRICDATA)DoubleVector.h
	g++ -I $(SOURCE_INDEX) $(CFLAGS) -c $(SOURCE_INDEX)BalancePartitionMethod.cpp

ExcludedMiddlePartitionMethod.o: $(SOURCE_INDEX)ExcludedMiddlePartitionMethod.cpp $(HEADER_INDEX)ExcludedMiddlePartitionMethod.h
	g++ -I $(SOURCE_INDEX) $(CFLAGS) -c $(SOURCE_INDEX)ExcludedMiddlePartitionMethod.cpp

FFTPivotSelectionMethod.o: $(SOURCE_INDEX)FFTPivotSelectionMethod.cpp $(HEADER_INDEX)FFTPivotSelectionMethod.h
	g++ -I $(SOURCE_INDEX) $(CFLAGS) -c $(SOURCE_INDEX)FFTPivotSelectionMethod.cpp

IncrementalSelection.o: $(SOURCE_INDEX)IncrementalSelection.cpp $(HEADER_INDEX)IncrementalSelection.h $(HEADER_METRICDATA)DoubleVector.h
	g++ -I $(SOURCE_INDEX) $(CFLAGS) -c $(SOURCE_INDEX)IncrementalSelection.cpp

PartitionResults.o: $(SOURCE_INDEX)PartitionResults.cpp $(HEADER_INDEX)PartitionResults.h
	g++ -I $(SOURCE_INDEX) $(CFLAGS) -c $(SOURCE_INDEX)PartitionResults.cpp

PCA.o: $(SOURCE_INDEX)PCA.cpp $(HEADER_INDEX)PCA.h $(HEADER_GSL)gsl_rng.h $(HEADER_GSL)gsl_matrix_double.h $(HEADER_GSL)gsl_vector_double.h $(HEADER_GSL)gsl_blas.h $(HEADER_GSL)gsl_eigen.h $(HEADER_GSL)gsl_linalg.h $(HEADER_GSL)gsl_statistics.h
	g++ -I $(SOURCE_INDEX) $(CFLAGS) -c $(SOURCE_INDEX)PCA.cpp 

#PCA.o: $(SOURCE_INDEX)PCA.cpp $(HEADER_INDEX)PCA.h gsl_rng.h gsl_matrix_double.h gsl_vector_double.h gsl_blas.h gsl_eigen.h gsl_linalg.h gsl_statistics.h
#	g++ -I $(SOURCE_INDEX) $(CFLAGS) -c $(SOURCE_INDEX)PCA.cpp `pkg-config --Cflags gsl`

#PCA.o: $(SOURCE_INDEX)PCA.cpp $(HEADER_INDEX)PCA.h
#	g++ -I $(SOURCE_INDEX) $(CFLAGS) -c $(SOURCE_INDEX)PCA.cpp `pkg-config --Cflags gsl`

PCAOnFFT.o: $(SOURCE_INDEX)PCAOnFFT.cpp $(HEADER_INDEX)PCAOnFFT.h $(HEADER_INDEX)IncrementalSelection.h $(HEADER_INDEX)FFTPivotSelectionMethod.h $(HEADER_INDEX)PCA.h $(HEADER_GSL)gsl_rng.h $(HEADER_GSL)gsl_matrix_double.h $(HEADER_GSL)gsl_vector_double.h $(HEADER_GSL)gsl_blas.h $(HEADER_GSL)gsl_eigen.h $(HEADER_GSL)gsl_linalg.h $(HEADER_GSL)gsl_statistics.h
	g++ -I $(SOURCE_INDEX) $(CFLAGS) -c $(SOURCE_INDEX)PCAOnFFT.cpp

#PCAOnFFT.o: $(SOURCE_INDEX)PCAOnFFT.cpp $(HEADER_INDEX)PCAOnFFT.h $(HEADER_INDEX)IncrementalSelection.h $(HEADER_INDEX)FFTPivotSelectionMethod.h $(HEADER_INDEX)PCA.h gsl_rng.h gsl_matrix_double.h gsl_vector_double.h gsl_blas.h gsl_eigen.h gsl_linalg.h gsl_statistics.h
#	g++ -I $(SOURCE_INDEX) $(CFLAGS) -c $(SOURCE_INDEX)PCAOnFFT.cpp `pkg-config --cflags gsl`

#PCAOnFFT.o: $(SOURCE_INDEX)PCAOnFFT.cpp $(HEADER_INDEX)PCAOnFFT.h $(HEADER_INDEX)IncrementalSelection.h $(HEADER_INDEX)FFTPivotSelectionMethod.h $(HEADER_INDEX)PCA.h
#	g++ -I $(SOURCE_INDEX) $(CFLAGS) -c $(SOURCE_INDEX)PCAOnFFT.cpp `pkg-config --cflags gsl`

CountedMetric.o: $(SOURCE_METRICDISTANCE)CountedMetric.cpp $(HEADER_METRICDISTANCE)CountedMetric.h
	g++ -I $(SOURCE_METRICDISTANCE) $(CFLAGS) -c $(SOURCE_METRICDISTANCE)CountedMetric.cpp

DNAMetric.o: $(SOURCE_METRICDISTANCE)DNAMetric.cpp $(HEADER_METRICDISTANCE)DNAMetric.h
	g++ -I $(SOURCE_METRICDISTANCE) $(CFLAGS) -c $(SOURCE_METRICDISTANCE)DNAMetric.cpp

EditDistance.o: $(SOURCE_METRICDISTANCE)EditDistance.cpp $(HEADER_METRICDISTANCE)EditDistance.h
	g++ -I $(SOURCE_METRICDISTANCE) $(CFLAGS) -c $(SOURCE_METRICDISTANCE)EditDistance.cpp

EuclideanDistance.o: $(SOURCE_METRICDISTANCE)EuclideanDistance.cpp $(HEADER_METRICDISTANCE)EuclideanDistance.h $(HEADER_METRICDATA)DoubleVector.h
	g++ -I $(SOURCE_METRICDISTANCE) $(CFLAGS) -c $(SOURCE_METRICDISTANCE)EuclideanDistance.cpp

ImageMetric.o: $(SOURCE_METRICDISTANCE)ImageMetric.cpp $(HEADER_METRICDISTANCE)ImageMetric.h
	g++ -I $(SOURCE_METRICDISTANCE) $(CFLAGS) -c $(SOURCE_METRICDISTANCE)ImageMetric.cpp

MetricDistance.o: $(SOURCE_METRICDISTANCE)MetricDistance.cpp $(HEADER_METRICDISTANCE)MetricDistance.h
	g++ -I $(SOURCE_METRICDISTANCE) $(CFLAGS) -c $(SOURCE_METRICDISTANCE)MetricDistance.cpp

PeptideMetric.o: $(SOURCE_METRICDISTANCE)PeptideMetric.cpp $(HEADER_METRICDISTANCE)PeptideMetric.h
	g++ -I $(SOURCE_METRICDISTANCE) $(CFLAGS) -c $(SOURCE_METRICDISTANCE)PeptideMetric.cpp

RNAMetric.o: $(SOURCE_METRICDISTANCE)RNAMetric.cpp $(HEADER_METRICDISTANCE)RNAMetric.h
	g++ -I $(SOURCE_METRICDISTANCE) $(CFLAGS) -c $(SOURCE_METRICDISTANCE)RNAMetric.cpp

DNA.o: $(SOURCE_METRICDATA)DNA.cpp $(HEADER_METRICDATA)DNA.h
	g++ -I $(SOURCE_METRICDATA) $(CFLAGS) -c $(SOURCE_METRICDATA)DNA.cpp

DoubleVector.o: $(SOURCE_METRICDATA)DoubleVector.cpp $(HEADER_METRICDATA)DoubleVector.h
	g++ -I $(SOURCE_METRICDATA) $(CFLAGS) -c $(SOURCE_METRICDATA)DoubleVector.cpp

Image.o: $(SOURCE_METRICDATA)Image.cpp $(HEADER_METRICDATA)Image.h
	g++ -I $(SOURCE_METRICDATA) $(CFLAGS) -c $(SOURCE_METRICDATA)Image.cpp

MetricData.o: $(SOURCE_METRICDATA)MetricData.cpp $(HEADER_METRICDATA)MetricData.h
	g++ -I $(SOURCE_METRICDATA) $(CFLAGS) -c $(SOURCE_METRICDATA)MetricData.cpp

Peptide.o: $(SOURCE_METRICDATA)Peptide.cpp $(HEADER_METRICDATA)Peptide.h
	g++ -I $(SOURCE_METRICDATA) $(CFLAGS) -c $(SOURCE_METRICDATA)Peptide.cpp

RNA.o: $(SOURCE_METRICDATA)RNA.cpp $(HEADER_METRICDATA)RNA.h
	g++ -I $(SOURCE_METRICDATA) $(CFLAGS) -c $(SOURCE_METRICDATA)RNA.cpp

StringObject.o: $(SOURCE_METRICDATA)StringObject.cpp $(HEADER_METRICDATA)StringObject.h
	g++ -I $(SOURCE_METRICDATA) $(CFLAGS) -c $(SOURCE_METRICDATA)StringObject.cpp

Query.o: $(SOURCE_QUERY)Query.cpp $(HEADER_QUERY)Query.h
	g++ -I $(SOURCE_QUERY) $(CFLAGS) -c $(SOURCE_QUERY)Query.cpp

RangeQuery.o: $(SOURCE_QUERY)RangeQuery.cpp $(HEADER_QUERY)RangeQuery.h
	g++ -I $(SOURCE_QUERY) $(CFLAGS) -c $(SOURCE_QUERY)RangeQuery.cpp

SearchTask.o: $(SOURCE_QUERY)SearchTask.cpp $(HEADER_QUERY)SearchTask.h $(HEADER_UTIL)ThreadPoolManager.h
	g++ -I $(SOURCE_QUERY) $(CFLAGS) -c $(SOURCE_QUERY)SearchTask.cpp

DoubleIndexObjectPair.o: $(SOURCE_METRICDATA)DoubleIndexObjectPair.cpp $(HEADER_METRICDATA)DoubleIndexObjectPair.h
	g++ -I $(SOURCE_METRICDATA) $(CFLAGS) -c $(SOURCE_METRICDATA)DoubleIndexObjectPair.cpp

getopt.o: $(SOURCE_UTIL)getopt.cpp $(HEADER_UTIL)getopt.h
	g++ -I $(SOURCE_UTIL) $(CFLAGS) -c $(SOURCE_UTIL)getopt.cpp

LoadOrSearch.o: $(SOURCE_UTIL)LoadOrSearch.cpp $(HEADER_METRICDISTANCE)MetricDistance.h $(HEADER_METRICDISTANCE)EuclideanDistance.h $(HEADER_INDEX)Index.h $(HEADER_INDEX)MVPIndex.h $(HEADER_INDEX)ListIndex.h $(HEADER_INDEX)PartitionMethod.h $(HEADER_INDEX)PivotSelectionMethod.h  $(HEADER_INDEX)PCAOnFFT.h $(HEADER_INDEX)FFTPivotSelectionMethod.h $(HEADER_INDEX)IncrementalSelection.h $(HEADER_INDEX)BalancePartitionMethod.h $(HEADER_QUERY)Query.h $(HEADER_QUERY)RangeQuery.h $(HEADER_METRICDISTANCE)DNAMetric.h $(HEADER_METRICDISTANCE)CountedMetric.h $(HEADER_METRICDISTANCE)EditDistance.h $(HEADER_METRICDISTANCE)ImageMetric.h $(HEADER_METRICDISTANCE)RNAMetric.h $(HEADER_METRICDISTANCE)PeptideMetric.h $(HEADER_QUERY)SearchTask.h $(HEADER_UTIL)MemMonitor.h $(HEADER_INDEX)ExcludedMiddleForest.h $(HEADER_INDEX)ExcludedMiddlePartitionMethod.h $(HEADER_UTIL)ThreadPoolManager.h
	g++ -I $(SOURCE_UTIL) $(CFLAGS) -c $(SOURCE_UTIL)LoadOrSearch.cpp

MVPIndexStructureAnalysor.o:$(SOURCE_UTIL)MVPIndexStructureAnalysor.cpp $(HEADER_INDEX)MVPIndex.h $(HEADER_INDEX)IndexNode.h $(HEADER_INDEX)MVPInternalNode.h $(HEADER_INDEX)MVPLeafNode.h $(HEADER_METRICDATA)MetricData.h $(HEADER_METRICDATA)DoubleVector.h $(HEADER_METRICDATA)DNA.h $(HEADER_METRICDATA)RNA.h $(HEADER_METRICDATA)Peptide.h $(HEADER_METRICDATA)StringObject.h $(HEADER_METRICDATA)Image.h $(HEADER_METRICDATA)TandemSpectra.h $(HEADER_QUERY)SearchTask.h $(HEADER_METRICDISTANCE)DNAMetric.h $(HEADER_METRICDISTANCE)CountedMetric.h $(HEADER_METRICDISTANCE)EditDistance.h $(HEADER_METRICDISTANCE)RNAMetric.h $(HEADER_METRICDISTANCE)PeptideMetric.h $(HEADER_METRICDISTANCE)EuclideanDistance.h
	g++ $(CFLAGS) -c $(SOURCE_UTIL)MVPIndexStructureAnalysor.cpp


ObjectFactory.o: $(SOURCE_UTIL)ObjectFactory.cpp $(HEADER_UTIL)ObjectFactory.h
	g++ -I $(SOURCE_UTIL) $(CFLAGS) -c $(SOURCE_UTIL)ObjectFactory.cpp

MemMonitor.o: $(SOURCE_UTIL)MemMonitor.cpp $(HEADER_UTIL)MemMonitor.h
	g++ -I $(SOURCE_UTIL) $(CFLAGS) -c $(SOURCE_UTIL)MemMonitor.cpp

ExcludedMiddleForest.o: $(SOURCE_INDEX)ExcludedMiddleForest.cpp $(HEADER_INDEX)ExcludedMiddleForest.h
	g++ -I $(SOURCE_INDEX) $(CFLAGS) -c $(SOURCE_INDEX)ExcludedMiddleForest.cpp

TandemSpectra.o:$(SOURCE_METRICDATA)TandemSpectra.cpp $(HEADER_METRICDATA)TandemSpectra.h
	g++ -I $(SOURCE_METRICDATA) $(CFLAGS) -c $(SOURCE_METRICDATA)TandemSpectra.cpp
	
ThreadPoolManager.o:$(SOURCE_UTIL)ThreadPoolManager.cpp $(HEADER_UTIL)ThreadPoolManager.h $(HEADER_UTIL)TaskDeclaration.h
	g++ -I $(HEADER_UTIL) -c $(SOURCE_UTIL)ThreadPoolManager.cpp $(CFLAGS)

LInfinityDistance.o:$(SOURCE_METRICDISTANCE)LInfinityDistance.cpp $(HEADER_METRICDISTANCE)LInfinityDistance.h
	g++ -I $(SOURCE_METRICDISTANCE) $(CFLAGS) -c $(SOURCE_METRICDISTANCE)LInfinityDistance.cpp

Clustering_Kmeans.o:$(SOURCE_CLUSTER_UTIL)Clustering_Kmeans.cpp $(HEADER_CLUSTER_UTIL)Clustering_Kmeans.h
	g++ -I $(SOURCE_CLUSTER_UTIL) $(CFLAGS) -c $(SOURCE_CLUSTER_UTIL)Clustering_Kmeans.cpp
	
ClusteringTask.o:$(SOURCE_CLUSTER_UTIL)ClusteringTask.cpp $(HEADER_CLUSTER_UTIL)ClusteringTask.h
	g++ -I $(SOURCE_CLUSTER_UTIL) $(CFLAGS) -c $(SOURCE_CLUSTER_UTIL)ClusteringTask.cpp
	
utilFunction.o:$(SOURCE_CLUSTER_UTIL)utilFunction.cpp $(HEADER_CLUSTER_UTIL)utilFunction.h
	g++ -I $(SOURCE_CLUSTER_UTIL) $(CFLAGS) -c $(SOURCE_CLUSTER_UTIL)utilFunction.cpp
	
PartitionByOnePivot_BalanceOne.o:$(SOURCE_CLUSTER)PartitionByOnePivot_BalanceOne.cpp $(HEADER_CLUSTER)PartitionByOnePivot_BalanceOne.h
	g++ -I $(SOURCE_CLUSTER) $(CFLAGS) -c $(SOURCE_CLUSTER)PartitionByOnePivot_BalanceOne.cpp
	
PartitionByOnePivot_Balance.o:$(SOURCE_CLUSTER)PartitionByOnePivot_Balance.cpp $(HEADER_CLUSTER)PartitionByOnePivot_Balance.h $(HEADER_CLUSTER_UTIL)utilFunction.h
	g++ -I $(SOURCE_CLUSTER) $(CFLAGS) -c $(SOURCE_CLUSTER)PartitionByOnePivot_Balance.cpp
	
PartitionByOnePivot_DBSCAN.o:$(SOURCE_CLUSTER)PartitionByOnePivot_DBSCAN.cpp $(HEADER_CLUSTER)PartitionByOnePivot_DBSCAN.h $(HEADER_CLUSTER_UTIL)utilFunction.h
	g++ -I $(SOURCE_CLUSTER) $(CFLAGS) -c $(SOURCE_CLUSTER)PartitionByOnePivot_DBSCAN.cpp
	
PartitionByOnePivot_Kmeans.o:$(SOURCE_CLUSTER)PartitionByOnePivot_Kmeans.cpp $(HEADER_CLUSTER)PartitionByOnePivot_Kmeans.h
	g++ -I $(SOURCE_CLUSTER) $(CFLAGS) -c $(SOURCE_CLUSTER)PartitionByOnePivot_Kmeans.cpp
	
PartitionByOnePivot_Trisection.o:$(SOURCE_CLUSTER)PartitionByOnePivot_Trisection.cpp $(HEADER_CLUSTER)PartitionByOnePivot_Trisection.h $(HEADER_CLUSTER_UTIL)utilFunction.h
	g++ -I $(SOURCE_CLUSTER) $(CFLAGS) -c $(SOURCE_CLUSTER)PartitionByOnePivot_Trisection.cpp

PivotWisePartitionMethod.o:$(SOURCE_CLUSTER)PivotWisePartitionMethod.cpp $(HEADER_CLUSTER)PivotWisePartitionMethod.h $(HEADER_CLUSTER_UTIL)Clustering_Kmeans.h $(HEADER_METRICDATA)DoubleVector.h $(HEADER_CLUSTER_UTIL)utilFunction.h $(HEADER_CLUSTER)PartitionByOnePivot_BalanceOne.h $(HEADER_CLUSTER)PartitionByOnePivot_Balance.h $(HEADER_CLUSTER)PartitionByOnePivot_DBSCAN.h $(HEADER_CLUSTER)PartitionByOnePivot_Kmeans.h $(HEADER_CLUSTER)PartitionByOnePivot_Trisection.h $(HEADER_CLUSTER)SelectOptimalPivots_Sequence.h $(HEADER_CLUSTER)SelectOptimalPivots_Variance.h
	g++ -I $(SOURCE_CLUSTER) $(CFLAGS) -c $(SOURCE_CLUSTER)PivotWisePartitionMethod.cpp

SelectOptimalPivots_Sequence.o:$(SOURCE_CLUSTER)SelectOptimalPivots_Sequence.cpp $(HEADER_CLUSTER)SelectOptimalPivots_Sequence.h
	g++ -I $(SOURCE_CLUSTER) $(CFLAGS) -c $(SOURCE_CLUSTER)SelectOptimalPivots_Sequence.cpp
	
SelectOptimalPivots_Variance.o:$(SOURCE_CLUSTER)SelectOptimalPivots_Variance.cpp $(HEADER_CLUSTER)PartitionByOnePivot_Kmeans.h $(HEADER_CLUSTER_UTIL)utilFunction.h
	g++ -I $(SOURCE_CLUSTER) $(CFLAGS) -c $(SOURCE_CLUSTER)SelectOptimalPivots_Variance.cpp

BuildAndSearchTypes.o: $(SOURCE_UTIL)BuildAndSearchTypes.cpp $(HEADER_METRICDATA)DoubleVector.h $(HEADER_METRICDISTANCE)MetricDistance.h $(HEADER_METRICDISTANCE)EuclideanDistance.h $(HEADER_INDEX)Index.h $(HEADER_INDEX)MVPIndex.h $(HEADER_INDEX)ListIndex.h $(HEADER_INDEX)PartitionMethod.h $(HEADER_INDEX)PivotSelectionMethod.h  $(HEADER_INDEX)PCAOnFFT.h $(HEADER_INDEX)FFTPivotSelectionMethod.h $(HEADER_INDEX)IncrementalSelection.h $(HEADER_INDEX)BalancePartitionMethod.h $(HEADER_QUERY)Query.h $(HEADER_QUERY)RangeQuery.h $(HEADER_METRICDISTANCE)DNAMetric.h $(HEADER_METRICDISTANCE)CountedMetric.h $(HEADER_METRICDISTANCE)EditDistance.h $(HEADER_METRICDISTANCE)ImageMetric.h $(HEADER_METRICDISTANCE)RNAMetric.h $(HEADER_METRICDISTANCE)PeptideMetric.h $(HEADER_QUERY)SearchTask.h $(HEADER_METRICDATA)Image.h $(HEADER_METRICDATA)DNA.h $(HEADER_METRICDATA)Peptide.h $(HEADER_METRICDATA)StringObject.h $(HEADER_UTIL)MemMonitor.h $(HEADER_INDEX)ExcludedMiddleForest.h $(HEADER_INDEX)ExcludedMiddlePartitionMethod.h $(HEADER_CLUSTER)PivotWisePartitionMethod.h
	g++ -I $(SOURCE_UTIL) $(CFLAGS) -c $(SOURCE_UTIL)BuildAndSearchTypes.cpp

MakeFileName.o:$(SOURCE_UTIL)MakeFileName.cpp
	g++ -I $(SOURCE_UTIL) $(CFLAGS) -c $(SOURCE_UTIL)MakeFileName.cpp
	
ReadLabel.o:$(SOURCE_CLASSIFIER_UTIL)ReadLabel.cpp $(HEADER_CLASSIFIER_UTIL)ReadLabel.h
	g++ -I $(HEADER_CLASSIFIER_UTIL) $(CFLAGS) -c $(SOURCE_CLASSIFIER_UTIL)ReadLabel.cpp

DatasetInMetricSpace.o:$(SOURCE_CLASSIFIER_UTIL)DatasetInMetricSpace.cpp $(HEADER_CLASSIFIER_UTIL)DatasetInMetricSpace.h $(HEADER_CLASSIFIER_UTIL)MetricDataFormat.h
	g++ -I $(SOURCE_CLASSIFIER_UTIL) $(CFLAGS) -c $(SOURCE_CLASSIFIER_UTIL)DatasetInMetricSpace.cpp

NavieBayes.o:$(SOURCE_CLASSIFIER_UTIL)NavieBayes.cpp $(HEADER_CLASSIFIER_UTIL)NavieBayes.h $(HEADER_CLASSIFIER_UTIL)MetricDataFormat.h
	g++ -I $(SOURCE_CLASSIFIER_UTIL) $(CFLAGS) -c $(SOURCE_CLASSIFIER_UTIL)NavieBayes.cpp

classification.o:$(SOURCE_CLASSIFIER_UTIL)classification.cpp $(HEADER_METRICDATA)MetricData.h $(HEADER_METRICDATA)DoubleVector.h $(HEADER_METRICDATA)Image.h $(HEADER_METRICDATA)StringObject.h $(HEADER_METRICDATA)DNA.h $(HEADER_METRICDATA)Peptide.h $(HEADER_METRICDISTANCE)MetricDistance.h $(HEADER_METRICDISTANCE)EuclideanDistance.h $(HEADER_METRICDISTANCE)EditDistance.h $(HEADER_METRICDISTANCE)ImageMetric.h $(HEADER_METRICDISTANCE)DNAMetric.h $(HEADER_METRICDISTANCE)PeptideMetric.h $(HEADER_METRICDISTANCE)LInfinityDistance.h $(HEADER_METRICDISTANCE)CountedMetric.h $(HEADER_INDEX)PivotSelectionMethod.h $(HEADER_INDEX)FFTPivotSelectionMethod.h $(HEADER_INDEX)PCAOnFFT.h $(HEADER_INDEX)IncrementalSelection.h $(HEADER_CLASSIFIER)Train_Knn.h $(HEADER_CLASSIFIER)Test_Knn.h $(HEADER_CLASSIFIER)Train_NavieBayes.h $(HEADER_CLASSIFIER)Test_NavieBayes.h $(HEADER_CLASSIFIER)Train_C4.5.h $(HEADER_CLASSIFIER)Test_C4.5.h $(HEADER_CLASSIFIER)Train.h $(HEADER_CLASSIFIER)Test.h $(HEADER_CLASSIFIER_UTIL)NavieBayes.h $(HEADER_CLASSIFIER_UTIL)DatasetInMetricSpace.h $(HEADER_CLASSIFIER_UTIL)MetricDataFormat.h $(HEADER_CLASSIFIER_UTIL)ReadLabel.h
	g++ -I $(SOURCE_CLASSIFIER_UTIL) $(CFLAGS) -c $(SOURCE_CLASSIFIER_UTIL)classification.cpp

TrainOrTest.o:$(SOURCE_CLASSIFIER_UTIL)TrainOrTest.cpp
	g++ -I $(SOURCE_CLASSIFIER_UTIL) $(CFLAGS) -c $(SOURCE_CLASSIFIER_UTIL)TrainOrTest.cpp

Train.o:$(SOURCE_CLASSIFIER)Train.cpp $(HEADER_CLASSIFIER)Train.h
	g++ -I $(HEADER_CLASSIFIER) $(CFLAGS) -c $(SOURCE_CLASSIFIER)Train.cpp

Train_Knn.o:$(SOURCE_CLASSIFIER)Train_Knn.cpp $(HEADER_CLASSIFIER)Train_Knn.h $(HEADER_CLASSIFIER_UTIL)MetricDataFormat.h
	g++ -I $(HEADER_CLASSIFIER) $(CFLAGS) -c $(SOURCE_CLASSIFIER)Train_Knn.cpp

Train_NavieBayes.o:$(SOURCE_CLASSIFIER)Train_NavieBayes.cpp $(HEADER_CLASSIFIER)Train_NavieBayes.h $(HEADER_CLASSIFIER_UTIL)GetValue.h $(HEADER_CLASSIFIER_UTIL)MetricDataFormat.h
	g++ -I $(SOURCE_CLASSIFIER) $(CFLAGS) -c $(SOURCE_CLASSIFIER)Train_NavieBayes.cpp
	
Train_C4.5.o:$(SOURCE_CLASSIFIER)Train_C4.5.cpp $(HEADER_CLASSIFIER)Train_C4.5.h $(HEADER_CLASSIFIER_UTIL)C4.5.h $(HEADER_CLASSIFIER_UTIL)MetricDataFormat.h
	g++ -I $(SOURCE_CLASSIFIER) $(CFLAGS) -c $(SOURCE_CLASSIFIER)Train_C4.5.cpp

Test.o:$(SOURCE_CLASSIFIER)Test.cpp $(HEADER_CLASSIFIER)Test.h
	g++ -I $(HEADER_CLASSIFIER) $(CFLAGS) -c $(SOURCE_CLASSIFIER)Test.cpp
	
Test_Knn.o:$(SOURCE_CLASSIFIER)Test_Knn.cpp $(HEADER_CLASSIFIER)Test_Knn.h $(HEADER_CLASSIFIER_UTIL)MetricDataFormat.h
	g++ -I $(HEADER_CLASSIFIER) $(CFLAGS) -c $(SOURCE_CLASSIFIER)Test_Knn.cpp

Test_NavieBayes.o:$(SOURCE_CLASSIFIER)Test_NavieBayes.cpp $(HEADER_CLASSIFIER)Test_NavieBayes.h $(HEADER_CLASSIFIER_UTIL)GetValue.h $(HEADER_CLASSIFIER_UTIL)MetricDataFormat.h
	g++ -I $(SOURCE_CLASSIFIER) $(CFLAGS) -c $(SOURCE_CLASSIFIER)Test_NavieBayes.cpp
	
Test_C4.5.o:$(SOURCE_CLASSIFIER)Test_C4.5.cpp $(HEADER_CLASSIFIER)Test_C4.5.h $(HEADER_CLASSIFIER_UTIL)C4.5.h $(HEADER_CLASSIFIER_UTIL)MetricDataFormat.h
	g++ -I $(SOURCE_CLASSIFIER) $(CFLAGS) -c $(SOURCE_CLASSIFIER)Test_C4.5.cpp

clean:
	rm *.o
