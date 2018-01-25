#include "x68sound_config.h"

#include "x68sound_global.h"
#include "x68sound_lfo.h"
#include "x68sound_context.internal.h"

#if X68SOUND_ENABLE_PORTABLE_CODE
Lfo::Lfo(X68SoundContextImpl *contextImpl) {
	m_contextImpl = contextImpl;
#else
Lfo::Lfo(void) {
#endif
	int i;

	for (i=0; i<N_CH; ++i) {
		Pmsmul[i] = 0;
		Pmsshl[i] = 0;
		Ams[i] = 31;
		PmdPmsmul[i] = 0;

		PmValue[i] = 0;
		AmValue[i] = 0;
	}
	Pmd = 0;
	Amd = 0;

	LfoStartingFlag = 0;
	LfoOverFlow = 0;
	LfoTime = 0;
	LfoTimeAdd = 0;
	LfoIdx = 0;
	LfoSmallCounter = 0;
	LfoSmallCounterStep = 0;
	Lfrq = 0;
	LfoWaveForm = 0;

	PmTblValue = 0;
	AmTblValue = 255;

	// PM Wave Form 0,3
	for (i=0; i<=127; ++i) {
		PmTbl0[i] = i;
		PmTbl0[i+128] = i-127;
		PmTbl0[i+256] = i;
		PmTbl0[i+384] = i-127;
	}
	// AM Wave Form 0,3
	for (i=0; i<=255; ++i) {
		AmTbl0[i] = 255-i;
		AmTbl0[i+256] = 255-i;
	}

	// PM Wave Form 2
	for (i=0; i<=127; ++i) {
		PmTbl2[i] = i;
		PmTbl2[i+128] = 127-i;
		PmTbl2[i+256] = -i;
		PmTbl2[i+384] = i-127;
	}
	// AM Wave Form 2
	for (i=0; i<=255; ++i) {
		AmTbl2[i] = 255-i;
		AmTbl2[i+256] = i;
	}

};

void Lfo::Init() {
	LfoTimeAdd = LFOPRECISION*62500/Samprate;

	LfoSmallCounter = 0;

	SetLFRQ(0);
	SetPMDAMD(0);
	SetPMDAMD(128+0);
	SetWaveForm(0);
	{
		int ch;
		for (ch=0; ch<N_CH; ++ch) {
			SetPMSAMS(ch, 0);
		}
	}
	LfoReset();
	LfoStart();
}
void Lfo::InitSamprate() {
	LfoTimeAdd = LFOPRECISION*62500/Samprate;
}

void Lfo::LfoReset() {
	LfoStartingFlag = 0;

//	LfoTime はリセットされない！！
	LfoIdx = 0;

	CulcTblValue();
	CulcAllPmValue();
	CulcAllAmValue();
}
void Lfo::LfoStart() {
	LfoStartingFlag = 1;
}
void Lfo::SetLFRQ(int n) {
	Lfrq = n & 255;

	LfoSmallCounterStep = 16+(Lfrq&15);
	int	shift = 15-(Lfrq>>4);
	if (shift == 0) {
		shift = 1;
		LfoSmallCounterStep <<= 1;
	}
	LfoOverFlow = (8<<shift) * LFOPRECISION;

//	LfoTime はリセットされる
	LfoTime = 0;
}
void Lfo::SetPMDAMD(int n) {
	if (n & 0x80) {
		Pmd = n&0x7F;
		int ch;
		for (ch=0; ch<N_CH; ++ch) {
			PmdPmsmul[ch] = Pmd * Pmsmul[ch];
		}
		CulcAllPmValue();
	} else {
		Amd = n&0x7F;
		CulcAllAmValue();
	}
}
void	Lfo::SetWaveForm(int n) {
	LfoWaveForm = n&3;

	CulcTblValue();
	CulcAllPmValue();
	CulcAllAmValue();
}
void	Lfo::SetPMSAMS(int ch, int n) {
	int pms = (n>>4)&7;
	Pmsmul[ch] = PMSMUL[pms];
	Pmsshl[ch] = PMSSHL[pms];
	PmdPmsmul[ch] = Pmd*Pmsmul[ch];
	CulcPmValue(ch);

	Ams[ch] = ((n&3)-1) & 31;
	CulcAmValue(ch);
}

void	Lfo::Update() {
	if (LfoStartingFlag == 0) {
		return;
	}

	LfoTime += LfoTimeAdd;
	if (LfoTime >= LfoOverFlow) {
		LfoTime = 0;
		LfoSmallCounter += LfoSmallCounterStep;
		switch (LfoWaveForm) {
		case 0:
			{
				int idxadd = LfoSmallCounter>>4;
				LfoIdx = (LfoIdx+idxadd) & (SIZELFOTBL-1);
				PmTblValue = PmTbl0[LfoIdx];
				AmTblValue = AmTbl0[LfoIdx];
				break;
			}
		case 1:
			{
				int idxadd = LfoSmallCounter>>4;
				LfoIdx = (LfoIdx+idxadd) & (SIZELFOTBL-1);
				if ((LfoIdx&(SIZELFOTBL/2-1)) < SIZELFOTBL/4) {
					PmTblValue = 128;
					AmTblValue = 256;
				} else {
					PmTblValue = -128;
					AmTblValue = 0;
				}
			}
			break;
		case 2:
			{
				int idxadd = LfoSmallCounter>>4;
				LfoIdx = (LfoIdx+idxadd+idxadd) & (SIZELFOTBL-1);
				PmTblValue = PmTbl2[LfoIdx];
				AmTblValue = AmTbl2[LfoIdx];
				break;
			}
		case 3:
			{
#if X68SOUND_ENABLE_PORTABLE_CODE
				LfoIdx = irnd(m_contextImpl) >> (32-SIZELFOTBL_BITS);
#else
				LfoIdx = irnd() >> (32-SIZELFOTBL_BITS);
#endif
				PmTblValue = PmTbl0[LfoIdx];
				AmTblValue = AmTbl0[LfoIdx];
				break;
			}
		}
		LfoSmallCounter &= 15;

		CulcAllPmValue();
		CulcAllAmValue();
	}
}


int Lfo::GetPmValue(int ch) {
	return PmValue[ch];
}
int Lfo::GetAmValue(int ch) {
	return AmValue[ch];
}

void Lfo::CulcTblValue() {
	switch (LfoWaveForm) {
	case 0:
		PmTblValue = PmTbl0[LfoIdx];
		AmTblValue = AmTbl0[LfoIdx];
		break;
	case 1:
		if ((LfoIdx&(SIZELFOTBL/2-1)) < SIZELFOTBL/4) {
			PmTblValue = 128;
			AmTblValue = 256;
		} else {
			PmTblValue = -128;
			AmTblValue = 0;
		}
		break;
	case 2:
		PmTblValue = PmTbl2[LfoIdx];
		AmTblValue = AmTbl2[LfoIdx];
		break;
	case 3:
		PmTblValue = PmTbl0[LfoIdx];
		AmTblValue = AmTbl0[LfoIdx];
		break;
	}
}
void	Lfo::CulcPmValue(int ch) {
	if (PmTblValue >= 0) {
		PmValue[ch] = ((PmTblValue*PmdPmsmul[ch])>>(7+5))<<Pmsshl[ch];
	} else {
		PmValue[ch] = -( (((-PmTblValue)*PmdPmsmul[ch])>>(7+5))<<Pmsshl[ch] );
	}
}
void	Lfo::CulcAmValue(int ch) {
	AmValue[ch] = (((AmTblValue*Amd)>>7)<<Ams[ch])&(int)0x7FFFFFFF;
}

void Lfo::CulcAllPmValue() {
	int ch;
	for (ch=0; ch<N_CH; ++ch) {
		CulcPmValue(ch);
	}
}
void Lfo::CulcAllAmValue() {
	int ch;
	for (ch=0; ch<N_CH; ++ch) {
		CulcAmValue(ch);
	}
}

