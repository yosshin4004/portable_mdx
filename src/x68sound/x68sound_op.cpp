#include "x68sound_config.h"

#include "x68sound_global.h"
#include "x68sound_op.h"
#include "x68sound_context.internal.h"

#if X68SOUND_ENABLE_PORTABLE_CODE
Op::Op(X68SoundContextImpl *contextImpl) {
	m_contextImpl = contextImpl;
#else
Op::Op(void) {
#endif
};

void Op::Init() {
	Note = 5*12+8;
	Kc = 5*16+8 + 1;
	Kf = 5;
	Ar = 10;
	D1r = 10;
	D2r = 5;
	Rr = 12;
	Ks = 1;
	Dt2 = 0;
	Dt1 = 0;

	ArTime = 0;
	Fl = 31;
	Fl_mask = 0;
	Out2Fb = 0;
	inp = 0;
	Inp_last = 0;
	DeltaT = 0;
	LfoPitch = CULC_DELTA_T;
	T = 0;
	LfoLevel = CULC_ALPHA;
	Alpha = 0;
	Tl = (128-127)<<3;
	Xr_el = 1024;
	Xr_step = 0;
	Mul = 2;
	Ame = 0;

//	NoiseStep = (__int64)(1<<26)*(__int64)62500/Samprate;
#if X68SOUND_ENABLE_PORTABLE_CODE
	NoiseStep = (int64_t)(1<<26)*(int64_t)OpmRate/Samprate;
#else
	NoiseStep = (__int64)(1<<26)*(__int64)OpmRate/Samprate;
#endif
	SetNFRQ(0);
	NoiseValue = 1;

	// 状態推移テーブルを作成
//	StatTbl[ATACK].nextstat = DECAY;
//	StatTbl[DECAY].nextstat = SUSTAIN;
//	StatTbl[SUSTAIN].nextstat = SUSTAIN_MAX;
//	StatTbl[SUSTAIN_MAX].nextstat = SUSTAIN_MAX;
//	StatTbl[RELEASE].nextstat = RELEASE_MAX;
//	StatTbl[RELEASE_MAX].nextstat = RELEASE_MAX;

	StatTbl[ATACK].limit = 0;
	StatTbl[DECAY].limit = D1LTBL[0];
	StatTbl[SUSTAIN].limit = 63;
	StatTbl[SUSTAIN_MAX].limit = 63;
	StatTbl[RELEASE].limit = 63;
	StatTbl[RELEASE_MAX].limit = 63;

#if X68SOUND_ENABLE_PORTABLE_CODE
	StatTbl[SUSTAIN_MAX].m_and = 4097;
#else
	StatTbl[SUSTAIN_MAX].and = 4097;
#endif
	StatTbl[SUSTAIN_MAX].cmp = 2048;
	StatTbl[SUSTAIN_MAX].add = 0;
#if X68SOUND_ENABLE_PORTABLE_CODE
	StatTbl[RELEASE_MAX].m_and = 4097;
#else
	StatTbl[RELEASE_MAX].and = 4097;
#endif
	StatTbl[RELEASE_MAX].cmp = 2048;
	StatTbl[RELEASE_MAX].add = 0;

	Xr_stat = RELEASE_MAX;
#if X68SOUND_ENABLE_PORTABLE_CODE
	Xr_and = StatTbl[Xr_stat].m_and;
#else
	Xr_and = StatTbl[Xr_stat].and;
#endif
	Xr_cmp = StatTbl[Xr_stat].cmp;
	Xr_add = StatTbl[Xr_stat].add;
	Xr_limit = StatTbl[Xr_stat].limit;

	CulcArStep();
	CulcD1rStep();
	CulcD2rStep();
	CulcRrStep();
	CulcPitch();
	CulcDt1Pitch();
}

void Op::InitSamprate() {
	LfoPitch = CULC_DELTA_T;

//	NoiseStep = (__int64)(1<<26)*(__int64)62500/Samprate;
#if X68SOUND_ENABLE_PORTABLE_CODE
	NoiseStep = (int64_t)(1<<26)*(int64_t)OpmRate/Samprate;
#else
	NoiseStep = (__int64)(1<<26)*(__int64)OpmRate/Samprate;
#endif
	CulcNoiseCycle();

	CulcArStep();
	CulcD1rStep();
	CulcD2rStep();
	CulcRrStep();
	CulcPitch();
	CulcDt1Pitch();
}

void	Op::CulcArStep() {
	if (Ar != 0) {
		int ks = (Ar<<1)+(Kc>>(5-Ks));
#if X68SOUND_ENABLE_PORTABLE_CODE
			StatTbl[ATACK].m_and = XRTBL[ks].m_and;
			StatTbl[ATACK].cmp = XRTBL[ks].m_and>>1;
#else
			StatTbl[ATACK].and = XRTBL[ks].and;
			StatTbl[ATACK].cmp = XRTBL[ks].and>>1;
#endif
		if (ks < 62) {
			StatTbl[ATACK].add = XRTBL[ks].add;
		} else {
			StatTbl[ATACK].add = 128;
		}
	} else {
#if X68SOUND_ENABLE_PORTABLE_CODE
		StatTbl[ATACK].m_and = 4097;
#else
		StatTbl[ATACK].and = 4097;
#endif
		StatTbl[ATACK].cmp = 2048;
		StatTbl[ATACK].add = 0;
	}
	if (Xr_stat == ATACK) {
#if X68SOUND_ENABLE_PORTABLE_CODE
		Xr_and = StatTbl[Xr_stat].m_and;
#else
		Xr_and = StatTbl[Xr_stat].and;
#endif
		Xr_cmp = StatTbl[Xr_stat].cmp;
		Xr_add = StatTbl[Xr_stat].add;
	}
};
void	Op::CulcD1rStep() {
	if (D1r != 0) {
		int ks = (D1r<<1)+(Kc>>(5-Ks));
#if X68SOUND_ENABLE_PORTABLE_CODE
		StatTbl[DECAY].m_and = XRTBL[ks].m_and;
		StatTbl[DECAY].cmp = XRTBL[ks].m_and>>1;
#else
		StatTbl[DECAY].and = XRTBL[ks].and;
		StatTbl[DECAY].cmp = XRTBL[ks].and>>1;
#endif
		StatTbl[DECAY].add = XRTBL[ks].add;
	} else {
#if X68SOUND_ENABLE_PORTABLE_CODE
		StatTbl[DECAY].m_and = 4097;
#else
		StatTbl[DECAY].and = 4097;
#endif
		StatTbl[DECAY].cmp = 2048;
		StatTbl[DECAY].add = 0;
	}
	if (Xr_stat == DECAY) {
#if X68SOUND_ENABLE_PORTABLE_CODE
		Xr_and = StatTbl[Xr_stat].m_and;
#else
		Xr_and = StatTbl[Xr_stat].and;
#endif
		Xr_cmp = StatTbl[Xr_stat].cmp;
		Xr_add = StatTbl[Xr_stat].add;
	}
};
void	Op::CulcD2rStep() {
	if (D2r != 0) {
		int ks = (D2r<<1)+(Kc>>(5-Ks));
#if X68SOUND_ENABLE_PORTABLE_CODE
		StatTbl[SUSTAIN].m_and = XRTBL[ks].m_and;
		StatTbl[SUSTAIN].cmp = XRTBL[ks].m_and>>1;
#else
		StatTbl[SUSTAIN].and = XRTBL[ks].and;
		StatTbl[SUSTAIN].cmp = XRTBL[ks].and>>1;
#endif
		StatTbl[SUSTAIN].add = XRTBL[ks].add;
	} else {
#if X68SOUND_ENABLE_PORTABLE_CODE
		StatTbl[SUSTAIN].m_and = 4097;
#else
		StatTbl[SUSTAIN].and = 4097;
#endif
		StatTbl[SUSTAIN].cmp = 2048;
		StatTbl[SUSTAIN].add = 0;
	}
	if (Xr_stat == SUSTAIN) {
#if X68SOUND_ENABLE_PORTABLE_CODE
		Xr_and = StatTbl[Xr_stat].m_and;
#else
		Xr_and = StatTbl[Xr_stat].and;
#endif
		Xr_cmp = StatTbl[Xr_stat].cmp;
		Xr_add = StatTbl[Xr_stat].add;
	}
};
void	Op::CulcRrStep() {
	int ks = (Rr<<2)+2+(Kc>>(5-Ks));
#if X68SOUND_ENABLE_PORTABLE_CODE
	StatTbl[RELEASE].m_and = XRTBL[ks].m_and;
	StatTbl[RELEASE].cmp = XRTBL[ks].m_and>>1;
#else
	StatTbl[RELEASE].and = XRTBL[ks].and;
	StatTbl[RELEASE].cmp = XRTBL[ks].and>>1;
#endif
	StatTbl[RELEASE].add = XRTBL[ks].add;
	if (Xr_stat == RELEASE) {
#if X68SOUND_ENABLE_PORTABLE_CODE
		Xr_and = StatTbl[Xr_stat].m_and;
#else
		Xr_and = StatTbl[Xr_stat].and;
#endif
		Xr_cmp = StatTbl[Xr_stat].cmp;
		Xr_add = StatTbl[Xr_stat].add;
	}
};
void Op::CulcPitch() {
	Pitch = (Note<<6)+Kf+Dt2;
}
void Op::CulcDt1Pitch() {
	Dt1Pitch = DT1TBL[(Kc&0xFC)+(Dt1&3)];
	if (Dt1&0x04) {
		Dt1Pitch = -Dt1Pitch;
	}
}

void Op::SetFL(int n) {
	n = (n>>3) & 7;
	if (n == 0) {
		Fl = 31;
		Fl_mask = 0;
	} else {
		Fl = (7-n+1+1);
		Fl_mask = -1;
	}
};

void Op::SetKC(int n) {
	Kc = n & 127;
	int note = Kc & 15;
	Note = ((Kc>>4)+1)*12+ note-(note>>2);
	++Kc;
	CulcPitch();
	CulcDt1Pitch();
	LfoPitch = CULC_DELTA_T;
	CulcArStep();
	CulcD1rStep();
	CulcD2rStep();
	CulcRrStep();
};

void Op::SetKF(int n) {
	Kf = (n&255)>>2;
	CulcPitch();
	LfoPitch = CULC_DELTA_T;
};

void Op::SetDT1MUL(int n) {
	Dt1 = (n>>4)&7;
	CulcDt1Pitch();
	Mul = (n&15)<<1;
	if (Mul == 0) {
		Mul = 1;
	}
	LfoPitch = CULC_DELTA_T;
};

void Op::SetTL(int n) {
	Tl = (128-(n&127))<<3;
	LfoLevel = CULC_ALPHA;
};

void Op::SetKSAR(int n) {
	Ks = (n&255)>>6;
	Ar = n & 31;
	CulcArStep();
	CulcD1rStep();
	CulcD2rStep();
	CulcRrStep();
};

void Op::SetAMED1R(int n) {
	D1r = n & 31;
	CulcD1rStep();
	Ame = 0;
	if (n & 0x80) {
		Ame = -1;
	}
};

void Op::SetDT2D2R(int n) {
	Dt2 = DT2TBL[(n&255)>>6];
	CulcPitch();
	LfoPitch = CULC_DELTA_T;
	D2r = n & 31;
	CulcD2rStep();
};

void Op::SetD1LRR(int n) {
	StatTbl[DECAY].limit = D1LTBL[(n&255)>>4];
	if (Xr_stat == DECAY) {
		Xr_limit = StatTbl[DECAY].limit;
	}

	Rr = n & 15;
	CulcRrStep();
};

void Op::KeyON() {
	if (Xr_stat >= RELEASE) {
		// KEYON
		T = 0;

		if (Xr_el == 0) {
			Xr_stat = DECAY;
#if X68SOUND_ENABLE_PORTABLE_CODE
			Xr_and = StatTbl[Xr_stat].m_and;
#else
			Xr_and = StatTbl[Xr_stat].and;
#endif
			Xr_cmp = StatTbl[Xr_stat].cmp;
			Xr_add = StatTbl[Xr_stat].add;
			Xr_limit = StatTbl[Xr_stat].limit;
			if ((Xr_el>>4) == Xr_limit) {
				Xr_stat = NEXTSTAT[Xr_stat];
#if X68SOUND_ENABLE_PORTABLE_CODE
				Xr_and = StatTbl[Xr_stat].m_and;
#else
				Xr_and = StatTbl[Xr_stat].and;
#endif
				Xr_cmp = StatTbl[Xr_stat].cmp;
				Xr_add = StatTbl[Xr_stat].add;
				Xr_limit = StatTbl[Xr_stat].limit;
			}
		} else {
			Xr_stat = ATACK;
#if X68SOUND_ENABLE_PORTABLE_CODE
			Xr_and = StatTbl[Xr_stat].m_and;
#else
			Xr_and = StatTbl[Xr_stat].and;
#endif
			Xr_cmp = StatTbl[Xr_stat].cmp;
			Xr_add = StatTbl[Xr_stat].add;
			Xr_limit = StatTbl[Xr_stat].limit;
		}
	}
};
void Op::KeyOFF() {
	Xr_stat = RELEASE;
#if X68SOUND_ENABLE_PORTABLE_CODE
	Xr_and = StatTbl[Xr_stat].m_and;
#else
	Xr_and = StatTbl[Xr_stat].and;
#endif
	Xr_cmp = StatTbl[Xr_stat].cmp;
	Xr_add = StatTbl[Xr_stat].add;
	Xr_limit = StatTbl[Xr_stat].limit;
	if ((Xr_el>>4) >= 63) {
		Xr_el = 1024;
		Xr_stat = MAXSTAT[Xr_stat];
#if X68SOUND_ENABLE_PORTABLE_CODE
		Xr_and = StatTbl[Xr_stat].m_and;
#else
		Xr_and = StatTbl[Xr_stat].and;
#endif
		Xr_cmp = StatTbl[Xr_stat].cmp;
		Xr_add = StatTbl[Xr_stat].add;
		Xr_limit = StatTbl[Xr_stat].limit;
	}
};

void Op::Envelope(int env_counter) {
	if ((env_counter&Xr_and) == Xr_cmp) {

		if (Xr_stat==ATACK) {
			// ATACK
			Xr_step += Xr_add;
			Xr_el += ((~Xr_el)*(Xr_step>>3)) >> 4;
			LfoLevel = CULC_ALPHA;
			Xr_step &= 7;

			if (Xr_el <= 0) {
				Xr_el = 0;
				Xr_stat = DECAY;
#if X68SOUND_ENABLE_PORTABLE_CODE
				Xr_and = StatTbl[Xr_stat].m_and;
#else
				Xr_and = StatTbl[Xr_stat].and;
#endif
				Xr_cmp = StatTbl[Xr_stat].cmp;
				Xr_add = StatTbl[Xr_stat].add;
				Xr_limit = StatTbl[Xr_stat].limit;
				if ((Xr_el>>4) == Xr_limit) {
					Xr_stat = NEXTSTAT[Xr_stat];
#if X68SOUND_ENABLE_PORTABLE_CODE
					Xr_and = StatTbl[Xr_stat].m_and;
#else
					Xr_and = StatTbl[Xr_stat].and;
#endif
					Xr_cmp = StatTbl[Xr_stat].cmp;
					Xr_add = StatTbl[Xr_stat].add;
					Xr_limit = StatTbl[Xr_stat].limit;
				}
			}
		} else {
			// DECAY, SUSTAIN, RELEASE
			Xr_step += Xr_add;
			Xr_el += Xr_step>>3;
			LfoLevel = CULC_ALPHA;
			Xr_step &= 7;

			int e = Xr_el>>4;
			if (e == 63) {
				Xr_el = 1024;
				Xr_stat = MAXSTAT[Xr_stat];
#if X68SOUND_ENABLE_PORTABLE_CODE
				Xr_and = StatTbl[Xr_stat].m_and;
#else
				Xr_and = StatTbl[Xr_stat].and;
#endif
				Xr_cmp = StatTbl[Xr_stat].cmp;
				Xr_add = StatTbl[Xr_stat].add;
				Xr_limit = StatTbl[Xr_stat].limit;
			} else if (e == Xr_limit) {
				Xr_stat = NEXTSTAT[Xr_stat];
#if X68SOUND_ENABLE_PORTABLE_CODE
				Xr_and = StatTbl[Xr_stat].m_and;
#else
				Xr_and = StatTbl[Xr_stat].and;
#endif
				Xr_cmp = StatTbl[Xr_stat].cmp;
				Xr_add = StatTbl[Xr_stat].add;
				Xr_limit = StatTbl[Xr_stat].limit;
			}
		}
	}
}

void Op::SetNFRQ(int nfrq) {
	if ((Nfrq ^ nfrq) & 0x80) {
		LfoLevel = CULC_ALPHA;
	}
	Nfrq = nfrq;
	CulcNoiseCycle();
}
void Op::CulcNoiseCycle() {
	if (Nfrq & 0x80) {
		NoiseCycle = (32-(Nfrq&31)) << 25;
		if (NoiseCycle < NoiseStep) {
			NoiseCycle = NoiseStep;
		}
		NoiseCounter = NoiseCycle;
	} else {
		NoiseCycle = 0;
	}
}


void Op::Output0(int lfopitch, int lfolevel) {
	if (LfoPitch != lfopitch) {
//		DeltaT = ((STEPTBL[Pitch+lfopitch]+Dt1Pitch)*Mul)>>1;
		DeltaT = ((STEPTBL[Pitch+lfopitch]+Dt1Pitch)*Mul)>>(6+1);
		LfoPitch = lfopitch;
	}
	T += DeltaT;

	int lfolevelame = lfolevel & Ame;
	if (LfoLevel != lfolevelame) {
		Alpha = (int)(ALPHATBL[ALPHAZERO+Tl-Xr_el-lfolevelame]);
		LfoLevel = lfolevelame;
	}
	int o = (Alpha)
		* (int)(SINTBL[(((T+Out2Fb)>>PRECISION_BITS))&(SIZESINTBL-1)]) ;

//	int o2 = (o+Inp_last) >> 1;
//	Out2Fb = (o+o) >> Fl;
	Out2Fb = ((o + Inp_last) & Fl_mask) >> Fl;
	Inp_last = o;

	*out = o;
	*out2 = o;	// alg=5用
	*out3 = o; // alg=5用
//	*out = o2;
//	*out2 = o2;	// alg=5用
//	*out3 = o2; // alg=5用
};

void Op::Output(int lfopitch, int lfolevel) {
	if (LfoPitch != lfopitch) {
//		DeltaT = ((STEPTBL[Pitch+lfopitch]+Dt1Pitch)*Mul)>>1;
		DeltaT = ((STEPTBL[Pitch+lfopitch]+Dt1Pitch)*Mul)>>(6+1);
		LfoPitch = lfopitch;
	}
	T += DeltaT;

	int lfolevelame = lfolevel & Ame;
	if (LfoLevel != lfolevelame) {
		Alpha = (int)(ALPHATBL[ALPHAZERO+Tl-Xr_el-lfolevelame]);
		LfoLevel = lfolevelame;
	}
	int o = (Alpha)
		* (int)(SINTBL[(((T+inp)>>PRECISION_BITS))&(SIZESINTBL-1)]) ;

	*out += o;
};

void Op::Output32(int lfopitch, int lfolevel) {
	if (LfoPitch != lfopitch) {
//		DeltaT = ((STEPTBL[Pitch+lfopitch]+Dt1Pitch)*Mul)>>1;
		DeltaT = ((STEPTBL[Pitch+lfopitch]+Dt1Pitch)*Mul)>>(6+1);
		LfoPitch = lfopitch;
	}
	T += DeltaT;

	int o;
	if (NoiseCycle == 0) {
		int lfolevelame = lfolevel & Ame;
		if (LfoLevel != lfolevelame) {
			Alpha = (int)(ALPHATBL[ALPHAZERO+Tl-Xr_el-lfolevelame]);
			LfoLevel = lfolevelame;
		}
		o = (Alpha)
			* (int)(SINTBL[(((T+inp)>>PRECISION_BITS))&(SIZESINTBL-1)]) ;
	} else {
		NoiseCounter -= NoiseStep;
		if (NoiseCounter <= 0) {
#if X68SOUND_ENABLE_PORTABLE_CODE
			NoiseValue = (int)((irnd(m_contextImpl)>>30)&2)-1;
#else
			NoiseValue = (int)((irnd()>>30)&2)-1;
#endif
			NoiseCounter += NoiseCycle;
		}

		int lfolevelame = lfolevel & Ame;
		if (LfoLevel != lfolevelame) {
			Alpha = (int)(NOISEALPHATBL[ALPHAZERO+Tl-Xr_el-lfolevelame]);
			LfoLevel = lfolevelame;
		}
		o = (Alpha)
			* NoiseValue * MAXSINVAL;
	}

	*out += o;
};

void Op::Output0_22(int lfopitch, int lfolevel) {
	if (LfoPitch != lfopitch) {
//		DeltaT = ((STEPTBL[Pitch+lfopitch]+Dt1Pitch)*Mul)>>1;
		DeltaT = ((STEPTBL[Pitch+lfopitch]+Dt1Pitch)*Mul)>>(6+1);
		LfoPitch = lfopitch;
	}
	T += DeltaT;

	int lfolevelame = lfolevel & Ame;
	if (LfoLevel != lfolevelame) {
		Alpha = (int)(ALPHATBL[ALPHAZERO+Tl-Xr_el-lfolevelame]);
		LfoLevel = lfolevelame;
	}
	int o = (Alpha)
		* (int)(SINTBL[(((T+Out2Fb)>>PRECISION_BITS))&(SIZESINTBL-1)]) ;

	Out2Fb = ((o + Inp_last) & Fl_mask) >> Fl;
	Inp_last = o;

//	*out += o;
//	*out2 += o;	// alg=5用
//	*out3 += o; // alg=5用
	*out = o;
	*out2 = o;	// alg=5用
	*out3 = o; // alg=5用
};

void Op::Output_22(int lfopitch, int lfolevel) {
	if (LfoPitch != lfopitch) {
//		DeltaT = ((STEPTBL[Pitch+lfopitch]+Dt1Pitch)*Mul)>>1;
		DeltaT = ((STEPTBL[Pitch+lfopitch]+Dt1Pitch)*Mul)>>(6+1);
		LfoPitch = lfopitch;
	}
	T += DeltaT;

	int lfolevelame = lfolevel & Ame;
	if (LfoLevel != lfolevelame) {
		Alpha = (int)(ALPHATBL[ALPHAZERO+Tl-Xr_el-lfolevelame]);
		LfoLevel = lfolevelame;
	}
	int o = (Alpha)
		* (int)(SINTBL[(((T+inp)>>PRECISION_BITS))&(SIZESINTBL-1)]) ;

	*out += o;
};

void Op::Output32_22(int lfopitch, int lfolevel) {
	if (LfoPitch != lfopitch) {
//		DeltaT = ((STEPTBL[Pitch+lfopitch]+Dt1Pitch)*Mul)>>1;
		DeltaT = ((STEPTBL[Pitch+lfopitch]+Dt1Pitch)*Mul)>>(6+1);
		LfoPitch = lfopitch;
	}
	T += DeltaT;

	int o;
	if (NoiseCycle == 0) {
		int lfolevelame = lfolevel & Ame;
		if (LfoLevel != lfolevelame) {
			Alpha = (int)(ALPHATBL[ALPHAZERO+Tl-Xr_el-lfolevelame]);
			LfoLevel = lfolevelame;
		}
		o = (Alpha)
			* (int)(SINTBL[(((T+inp)>>PRECISION_BITS))&(SIZESINTBL-1)]) ;
	} else {
		NoiseCounter -= NoiseStep;
		if (NoiseCounter <= 0) {
#if X68SOUND_ENABLE_PORTABLE_CODE
			NoiseValue = (int)((irnd(m_contextImpl)>>30)&2)-1;
#else
			NoiseValue = (int)((irnd()>>30)&2)-1;
#endif
			NoiseCounter += NoiseCycle;
		}

		int lfolevelame = lfolevel & Ame;
		if (LfoLevel != lfolevelame) {
			Alpha = (int)(NOISEALPHATBL[ALPHAZERO+Tl-Xr_el-lfolevelame]);
			LfoLevel = lfolevelame;
		}
#if X68SOUND_ENABLE_PORTABLE_CODE
		o = (Alpha)
			* NoiseValue * MAXSINVAL;
#else
		int o = (Alpha)
			* NoiseValue * MAXSINVAL;
#endif
	}

	*out += o;
};

