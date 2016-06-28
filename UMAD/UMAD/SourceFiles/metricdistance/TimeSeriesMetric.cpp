
#include "../../HeaderFiles/metricdistance/TimeSeriesMetric.h"


/**

 * This class define a metric to compute distance of two time series sequence.

 */

	/**

     * @brief A no parameter constructor, do nothing.

     * @return void

     */
    CTimeSeriesMetric::CTimeSeriesMetric()
	{
		onelen = 0;
		twolen = 0;
		onecoord = 0;
		twocoord = 0;
	}


    /** 

     * @brief A destructor, do nothing

     * @return void

     */
    CTimeSeriesMetric::~CTimeSeriesMetric()
	{

	}

	 /**

     * @brief This method return two IndexObjects' distance.

     * @return  Return a double type distance of two objects.

     */
	double CTimeSeriesMetric::getDistance(CMetricData* one, CMetricData* two)
	{
		#ifdef CHECKPOINTER

		if(_obj1==NULL || _obj2==NULL)

			cout<<"null pointer parameters"<<endl;

		#endif

		CTimeSeries* new_one = static_cast<CTimeSeries*>(one);

		CTimeSeries* new_two = static_cast<CTimeSeries*>(two);


		return getDistance(new_one,new_two);

	}

	/**

     * @brief  This method return two time series' distance.

     *          Sum up edit distance of two time series.

     * @return   Return a double type of distance of two objects.

     */
	double CTimeSeriesMetric::getDistance(CTimeSeries* one, CTimeSeries* two)
	{
		onedata = one->getData();
		twodata = two->getData();
		onelen = one->getLen();
		twolen = two->getLen();
		onecoord = one->getCoord();
		twocoord = two->getCoord();

		//return(DTWDistance(onedata,twodata));

		//return(LCSSDistance(onedata,twodata));

		//return(EDRDistance(onedata,twodata));

		//return(ERPDistance(onedata,twodata));

		//return(EuclideanDistance(onedata,twodata));

		return(TraSegmentDistance(onedata,twodata));

	}



	/**
	 
	 *brief This method return two trajectory segments distance

	*/
	double CTimeSeriesMetric::TraSegmentDistance(vector<TimeSeries> one,vector<TimeSeries> two)
	{
		double distance = 0;
		TimeSeries Si = one[0];
		TimeSeries Ei = one[1];
		TimeSeries Sj = two[0];
		TimeSeries Ej = two[1];
		double verticalDis = 0;
		double parallelDis = 0;
		double angleDis = 0;
		TimeSeries SiEi,SiSj,SiEj,SjEj;
		SiEi.x_coord = Ei.x_coord - Si.x_coord;
		SiEi.y_coord = Ei.y_coord - Si.y_coord;
		SiEi.z_coord = 0;
		SiEi.time = 0;
		SiSj.x_coord = Sj.x_coord - Si.x_coord;
		SiSj.y_coord = Sj.y_coord - Si.y_coord;
		SiSj.z_coord = 0;
		SiSj.time = 0;
		SiEj.x_coord = Ej.x_coord - Si.x_coord;
		SiEj.y_coord = Ej.y_coord - Si.y_coord;
		SiEj.z_coord = 0;
		SiEj.time = 0;
		SjEj.x_coord = Ej.x_coord - Sj.x_coord;
		SjEj.y_coord = Ej.y_coord - Sj.y_coord;
		SjEj.z_coord = 0;
		SjEj.time = 0;
		double SiEiLen,SjEjLen;
		SiEiLen = sqrt(pow(SiEi.x_coord,2)+pow(SiEi.y_coord,2));
		SjEjLen = sqrt(pow(SjEj.x_coord,2)+pow(SjEj.y_coord,2));
		double SiSjandSiEi,SiEjandSiEi;
		SiSjandSiEi = SiSj.x_coord*SiEi.x_coord + SiSj.y_coord*SiEi.y_coord;
		SiEjandSiEi = SiEj.x_coord*SiEi.x_coord + SiEj.y_coord*SiEi.y_coord;
		double u1,u2;
		if (SiEiLen == 0)
		{
			u1 = 0;
			u2 = 0;
		}
		else
		{
			u1 = SiSjandSiEi/pow(SiEiLen,2);
			u2 = SiEjandSiEi/pow(SiEiLen,2);
		}
		
		TimeSeries Ps,Pe;
		Ps.x_coord = Si.x_coord + u1*SiEi.x_coord;
		Ps.y_coord = Si.y_coord + u1*SiEi.y_coord;
		Ps.z_coord = 0;
		Ps.time = 0;
		Pe.x_coord = Si.x_coord + u2*SiEi.x_coord;
		Pe.y_coord = Si.y_coord + u2*SiEi.y_coord;
		Pe.z_coord = 0;
		Pe.time = 0;
		double cosAngle;
		if((SiEiLen*SjEjLen) == 0)
		{
			cosAngle = 0;
		}
		else
		{
			cosAngle = (SiEi.x_coord*SjEj.x_coord + SiEi.y_coord*SjEj.y_coord)/(SiEiLen*SjEjLen);
		}

		double VecticalL1,VecticalL2;
		VecticalL1 = sqrt(pow(Sj.x_coord - Ps.x_coord,2)+pow(Sj.y_coord - Ps.y_coord,2));
		VecticalL2 = sqrt(pow(Ej.x_coord - Pe.x_coord,2)+pow(Ej.y_coord - Pe.y_coord,2));
		if((VecticalL1+VecticalL2) == 0)
		{
			verticalDis = 0;
		}
		else
		{
			verticalDis = (pow(VecticalL1,2)+pow(VecticalL2,2))/(VecticalL1+VecticalL2);
		}

		double ParalleL1,ParalleL2;
		ParalleL1 = sqrt(pow(Si.x_coord - Ps.x_coord,2)+pow(Si.y_coord - Ps.y_coord,2));
		ParalleL2 = sqrt(pow(Pe.x_coord - Ei.x_coord,2)+pow(Pe.y_coord - Ei.y_coord,2));
		parallelDis = MIN(ParalleL1,ParalleL2);

		double angle;
		angle = acos(cosAngle);
		if(angle >= 0 && angle < 90)
			angleDis = SjEjLen * sin(angle);
		else if(angle >= 90 && angle <= 180)
			angleDis = SjEjLen;
		else
			angleDis = 0;

		distance = verticalDis + parallelDis + angleDis;
		return distance;
	}





	/**
	 
	 *brief This method return two time series's DTW distance

	*/
	double CTimeSeriesMetric::DTWDistance(vector<TimeSeries> one, vector<TimeSeries> two)
	{
		double DTW[MAXNUM][MAXNUM];
		for(int i=0; i<=onelen; ++i)
			DTW[i][0] = INFINITY;
		for(int j=0; j<=twolen; ++j)
			DTW[0][j] = INFINITY;
		DTW[0][0] = 0;

		for(int i=1; i<=onelen; ++i)
		{
			for(int j=1; j<=twolen; ++j)
			{
				double cost;
				cost = DTWDist(one[i-1],two[j-1]);
				DTW[i][j] = cost + Minmum(DTW[i-1][j],DTW[i][j-1],DTW[i-1][j-1]);
			}
		}
			
		return DTW[onelen][twolen];
		/** initional the zero row of DTW = 0 **/
		/*vector<vector<double> > DTW;
		vector<double> DTW_row;*/
		/*for(int i=0; i<twolen; ++i)
			DTW_row.push_back(0);
		DTW.push_back(DTW_row);
		DTW_row.clear();*/
		/*for(int j=1; j<onelen; ++j)
		{
			DTW_row.push_back(0);
			DTW.push_back(DTW_row);
			DTW_row.clear();
		}
		DTW_row.clear();*/
	}


	/**

	 *brief This method return absolute distance between two point

	*/
	double CTimeSeriesMetric::DTWDist(TimeSeries t1, TimeSeries t2)
	{
		if(onecoord == twocoord == 1)
			
			return abs(t1.x_coord-t2.x_coord);
		
		else if(onecoord == twocoord == 2)

			return abs(t1.x_coord-t2.x_coord)+abs(t1.y_coord-t2.y_coord);

		else if(onecoord == twocoord == 3)

			return abs(t1.x_coord-t2.x_coord)+abs(t1.y_coord-t2.y_coord)+abs(t1.z_coord-t2.z_coord);

		else

			return abs(t1.x_coord-t2.x_coord)+abs(t1.y_coord-t2.y_coord)+abs(t1.z_coord-t2.z_coord)+abs(t1.time-t2.time); 
	}


	/**
	 
	 *brief This method return two time series's LCSS(Longest Common SubSequence) distance

	*/
	double CTimeSeriesMetric::LCSSDistance(vector<TimeSeries> one, vector<TimeSeries> two)
	{
		double LCSS[MAXNUM][MAXNUM];
		for(int i=0; i<=onelen; ++i)
			LCSS[i][0] = 0;
		for(int j=0; j<=twolen; ++j)
			LCSS[0][j] = 0;

		for(int i=1; i<=onelen; ++i)
		{
			for(int j=1; j<=twolen; ++j)
			{
				if(onecoord == twocoord == 1)
				{
					if(abs(one[i-1].x_coord-two[j-1].x_coord) <= LCSSorEDR)
					{
						LCSS[i][j] = LCSS[i-1][j-1] + 1;
					}
					else
					{
						LCSS[i][j] = MAX(LCSS[i-1][j],LCSS[i][j-1]);
					}
				}
				else if(onecoord == twocoord == 2)
				{
					if(abs(one[i-1].x_coord-two[j-1].x_coord) <= LCSSorEDR && abs(one[i-1].y_coord-two[j-1].y_coord) <= LCSSorEDR)
					{
						LCSS[i][j] = LCSS[i-1][j-1] + 1;
					}
					else
					{
						LCSS[i][j] = MAX(LCSS[i-1][j],LCSS[i][j-1]);
					}
				}
				else if(onecoord == twocoord == 3)
				{
					if(abs(one[i-1].x_coord-two[j-1].x_coord) <= LCSSorEDR && abs(one[i-1].y_coord-two[j-1].y_coord) <= LCSSorEDR && abs(one[i-1].z_coord-two[j-1].z_coord) <= LCSSorEDR)
					{
						LCSS[i][j] = LCSS[i-1][j-1] + 1;
					}
					else
					{
						LCSS[i][j] = MAX(LCSS[i-1][j],LCSS[i][j-1]);
					}
				}
				else if(onecoord == twocoord == 4)
				{
					if(abs(one[i-1].x_coord-two[j-1].x_coord) <= LCSSorEDR && abs(one[i-1].y_coord-two[j-1].y_coord) <= LCSSorEDR && abs(one[i-1].z_coord-two[j-1].z_coord) <= LCSSorEDR && abs(one[i-1].time-two[j-1].time) <= LCSSorEDR)
					{
						LCSS[i][j] = LCSS[i-1][j-1] + 1;
					}
					else
					{
						LCSS[i][j] = MAX(LCSS[i-1][j],LCSS[i][j-1]);
					}
				}
			}
		}
		return LCSS[onelen][twolen];
	}


	/**
	
	 *brief This method return the minmum of three parameter
	
	*/
	double CTimeSeriesMetric::Minmum(double x, double y, double z)
	{
		double temp;
		temp = MIN(x,y);
		return MIN(temp,z);
	}


	/**
	 
	 *brief This method return two time series's EDR(Edit Distance on Real Sequence) distance

	*/
	double CTimeSeriesMetric::EDRDistance(vector<TimeSeries> one, vector<TimeSeries> two)
	{
		TimeSeries gap ;
		gap.time = 0;
		gap.x_coord = 0;
		gap.y_coord = 0;
		gap.z_coord = 0;

		double EDR[MAXNUM][MAXNUM];
		for(int i=0; i<=onelen; ++i)
			EDR[i][0] = twolen;
		for(int j=0; j<=twolen; ++j)
			EDR[0][j] = onelen;
		EDR[0][0] = 0;

		for(int i=1; i<=onelen; ++i)
		{
			for(int j=1; j<=twolen; ++j)
			{
				if(one[i-1].x_coord == 0 || two[j-1].x_coord == 0)
				{
					EDR[i][j] = EDR[i-1][j-1];
				}
				else
				{
					double ij = EDRDist(one[i-1],two[j-1]);
					double igap = EDRDist(one[i-1],gap);
					double gapj = EDRDist(gap,two[j-1]);
					EDR[i][j] = Minmum((EDR[i-1][j-1]+ij), (EDR[i-1][j]+igap), (EDR[i][j-1]+gapj));
				}
			}
		}
		return EDR[onelen][twolen];
	}


	/**

	 *brief This method return distance between two point

	*/
	double CTimeSeriesMetric::EDRDist(TimeSeries t1, TimeSeries t2)
	{
		if(onecoord == twocoord == 1)
		{
			if(abs(t1.x_coord-t2.x_coord) <= LCSSorEDR)
			{
				return 0;
			}
			else 
			{
				return 1;
			}
		}
		else if(onecoord == twocoord == 2)
		{
			if(abs(t1.x_coord-t2.x_coord) <= LCSSorEDR && abs(t1.y_coord-t2.y_coord) <= LCSSorEDR)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else if(onecoord == twocoord == 3)
		{
			if(abs(t1.x_coord-t2.x_coord) <= LCSSorEDR && abs(t1.y_coord-t2.y_coord) <= LCSSorEDR && abs(t1.z_coord-t2.z_coord) <= LCSSorEDR)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else if(onecoord == twocoord == 4)
		{
			if(abs(t1.x_coord-t2.x_coord) <= LCSSorEDR && abs(t1.y_coord-t2.y_coord) <= LCSSorEDR && abs(t1.z_coord-t2.z_coord) <= LCSSorEDR && abs(t1.time-t2.time) <= LCSSorEDR)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		return 0;
	}

	/**
	 
	 *brief This method return two time series's ERP(Edit Distance with Real Penalty) distance

	*/
	double CTimeSeriesMetric::ERPDistance(vector<TimeSeries> one, vector<TimeSeries> two)
	{
		TimeSeries gap ;
		gap.time = 0;
		gap.x_coord = 0;
		gap.y_coord = 0;
		gap.z_coord = 0;

		double ERP[MAXNUM][MAXNUM];
		for(int i=1; i<=onelen; ++i)
			ERP[i][0] = ERPDist(one[i-1],gap);
		for(int j=1; j<=twolen; ++j)
			ERP[0][j] = ERPDist(gap,two[j-1]);
		ERP[0][0] = 0;

		for(int i=1; i<=onelen; ++i)
		{
			for(int j=1; j<=twolen; ++j)
			{
				double ij = ERPDist(one[i-1],two[j-1]);
				double igap = ERPDist(one[i-1],gap);
				double gapj = ERPDist(gap,two[j-1]);
				ERP[i][j] = Minmum((ERP[i-1][j-1]+ij), (ERP[i-1][j]+igap), (ERP[i][j-1]+gapj));
			}
		}
		return ERP[onelen][twolen];
	}

	/**

	 *brief This method return distance between two point

	*/
	double CTimeSeriesMetric::ERPDist(TimeSeries t1, TimeSeries t2)
	{
		if(onecoord == twocoord == 1)
		{
			if(t1.x_coord != 0 && t2.x_coord != 0)
			{
				return abs(t1.x_coord-t2.x_coord);
			}
			else if(t2.x_coord == 0)
			{
				return abs(t1.x_coord-t2.x_coord);
			}
			else if(t1.x_coord == 0)
			{
				return abs(t1.x_coord-t2.x_coord);
			}
		}
		else if(onecoord == twocoord == 2)
		{
			if(t1.x_coord != 0 && t1.y_coord != 0 && t2.x_coord != 0 && t2.y_coord != 0)
			{
				return abs(t1.x_coord-t2.x_coord)+abs(t1.y_coord-t2.y_coord);
			}
			else if( t2.x_coord== 0 && t2.y_coord == 0)
			{
				return abs(t1.x_coord-t2.x_coord)+abs(t1.y_coord-t2.y_coord);
			}
			else if(t1.x_coord == 0 && t1.y_coord == 0)
			{
				return abs(t1.x_coord-t2.x_coord)+abs(t1.y_coord-t2.y_coord);
			}
		}
		else if(onecoord == twocoord == 3)
		{
			if(t1.x_coord != 0 && t1.y_coord != 0 && t1.z_coord != 0 && t2.x_coord != 0 && t2.y_coord != 0 && t2.z_coord != 0)
			{
				return abs(t1.x_coord-t2.x_coord)+abs(t1.y_coord-t2.y_coord)+abs(t1.z_coord-t2.z_coord);
			}
			else if(t2.x_coord == 0 && t2.y_coord == 0 && t2.z_coord == 0)
			{
				return abs(t1.x_coord-t2.x_coord)+abs(t1.y_coord-t2.y_coord)+abs(t1.z_coord-t2.z_coord);
			}
			else if(t1.x_coord == 0 && t1.y_coord == 0 && t1.z_coord == 0)
			{
				return abs(t1.x_coord-t2.x_coord)+abs(t1.y_coord-t2.y_coord)+abs(t1.z_coord-t2.z_coord);
			}
		}
		else if(onecoord == twocoord == 4)
		{
			if(t1.x_coord != 0 && t1.y_coord != 0 && t1.z_coord != 0 && t1.time != 0 && t2.x_coord != 0 && t2.y_coord != 0 && t2.z_coord != 0 && t1.time != 0)
			{
				return abs(t1.x_coord-t2.x_coord)+abs(t1.y_coord-t2.y_coord)+abs(t1.z_coord-t2.z_coord)+abs(t1.time-t2.time);
			}
			else if(t2.x_coord == 0 && t2.y_coord == 0 && t2.z_coord == 0 && t2.time == 0)
			{
				return abs(t1.x_coord-t2.x_coord)+abs(t1.y_coord-t2.y_coord)+abs(t1.z_coord-t2.z_coord)+abs(t1.time-t2.time);
			}
			else if(t1.x_coord == 0 && t1.y_coord ==0 && t1.z_coord == 0 && t1.time == 0)
			{
				return abs(t1.x_coord-t2.x_coord)+abs(t1.y_coord-t2.y_coord)+abs(t1.z_coord-t2.z_coord)+abs(t1.time-t2.time);
			}
		}
		return 0;
	}


	/**
	 
	 *brief This method return two time series's Euclidean Distance

	*/
	double CTimeSeriesMetric::EuclideanDistance(vector<TimeSeries> one, vector<TimeSeries> two)
	{
		double distance = 0;
		for(int i=0; i<onelen; ++i)
		{
			for(int j=0; j<twolen; ++j)
			{
				if(onecoord == twocoord == 1)
				{
					distance += sqrt(pow(one[i].x_coord-two[j].x_coord,2));
				}
				else if(onecoord == twocoord == 2)
				{
					distance += sqrt(pow(one[i].x_coord-two[j].x_coord,2)+pow(one[i].y_coord-two[j].y_coord,2));
				}
				else if(onecoord == twocoord == 3)
				{
					distance += sqrt(pow(one[i].x_coord-two[j].x_coord,2)+pow(one[i].y_coord-two[j].y_coord,2)+pow(one[i].z_coord-two[j].z_coord,2));
				}
				else if(onecoord == twocoord == 4)
				{
					distance += sqrt(pow(one[i].x_coord-two[j].x_coord,2)+pow(one[i].y_coord-two[j].y_coord,2)+pow(one[i].z_coord-two[j].z_coord,2)+pow(one[i].time-two[j].time,2));
				}
			}
		}
		return distance;
	}