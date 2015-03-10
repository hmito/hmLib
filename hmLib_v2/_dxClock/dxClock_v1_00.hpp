		Fps=0.;
		PreTime=GetNowCount();
		IniTime=PreTime;
		IniLocalTime=hm::time();
		Time=PreTime;
		Cnt=0;

		//ˆ—‘Ò‚¿ŠÔ(ms)
		WaitTimer(_DrawIntervalTime);

		//TimeŠÖ˜Aˆ—
		Cnt++;
		Time=GetNowCount();
		if(Cnt%10==0){
			Fps=10000./((double)(Time-PreTime));
			PreTime=Time;
		}