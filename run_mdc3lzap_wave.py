from Gaudi.Configuration import *
import os

try:
  inputFiles = os.environ['LZAP_INPUT_FILES'].split(',')
except KeyError:
   with open ("/global/homes/e/ericmo/general-analysis/mdc3/data/driftplot_back_sparsedata.txt", "r") as myfile:
             inputFiles=myfile.read().split('\n')
#try:
#  inputFiles = os.environ['LZAP_INPUT_FILES'].split(',')
#except KeyError:
#  inputFiles = [ '/global/projecta/projectdirs/lz/data/MDC3tests/x86_64-slc6-gcc48-opt/background/BACCARAT-4.5.9_DER-8.4.5/20170403/DER/lz_201704030000_000000_000000_raw.root' ]
    
try:
    outputFile = os.environ['LZAP_OUTPUT_FILE']
except KeyError:
    outputFile = 'lzap_waveforms.root'

try:
    useDbi = True if os.environ['LZAP_USE_DBI'].lower() == "true" else False
except KeyError:
    useDbi = False

mcOutputFile = outputFile.replace('.root', '_mctruth.root')
    
app=ApplicationMgr()
app.EvtMax  = -1
app.HistogramPersistency = "NONE"
app.EvtSel = "Ldrf"
app.OutputLevel = 3


#################################################
# Open Ldrf (DER) file(s)
from DaqLdrfModules import DaqLdrfModulesConf
selector=DaqLdrfModulesConf.DaqLdrf__LdrfSelector("EventSelector")
#selector.InputFiles = inputFiles
selector.SparseEventsFile = "/global/homes/e/ericmo/general-analysis/mdc3/data/driftplot_back_sparsedata.txt"
app.SvcMapping.append('DaqLdrf::LdrfSelector/EventSelector')
app.SvcMapping.append('DaqLdrf::LdrfConversionSvc/LdrfConversionSvc')
app.SvcMapping.append('LZap::LzConditionsSvc/LzConditionsSvc')
app.SvcMapping.append('RQ::RQConversionSvc/RQConversionSvc')
app.SvcMapping.append('RQ::RQMCTruthConversionSvc/RQMCTruthConversionSvc')
app.SvcMapping.append('LZap::LzEvtContextSvc/LzEvtContextSvc')

from LZapServices import LZapServicesConf
condSvc=LZapServicesConf.LZap__LzConditionsSvc('LzConditionsSvc')
if useDbi:
  condSvc.UseDbi=useDbi
  # If any of the following are non-null strings, then that text file will take precedence over the DB
  condSvc.PMTpositionsFile=''
  condSvc.ElectronLifetimesFile=''
  condSvc.PositionCorrectionMapFile=''
  condSvc.ChannelResponseFile=''
  condSvc.ChannelOffsetsFile=''

from GaudiCommonSvc import GaudiCommonSvcConf
ipersist=GaudiCommonSvcConf.PersistencySvc('EventPersistencySvc')
ipersist.CnvServices=['LdrfConversionSvc']

persist=GaudiCommonSvcConf.PersistencySvc('PersistencySvc')
persist.CnvServices=['RQConversionSvc', 'RQMCTruthConversionSvc']

from RQModules import RQModulesConf
rqConversionSvc = RQModulesConf.RQ__RQConversionSvc('RQConversionSvc')
rqConversionSvc.SaveRQMCTruth = True
rqConversionSvc.SavePodWaveforms = True
outStream=RQModulesConf.RQ__RQOutputStream('RQWriter')
outStream.OutputFile = outputFile
outStream.OptItemList = [ '/Event/Calibrated',
                          '/Event/TPC/Physics/Pulses/Boundaries',
                          '/Event/TPC/Physics/Pulses/Parameters',
                          '/Event/TPC/Physics/Pulses/ChannelPulseParameters',
                          '/Event/TPC/Physics/Pulses/Classifications',
                          '/Event/TPC/Physics/Pulses/ChannelPulsePhotons',
                          '/Event/TPC/Physics/Pulses/Photons',
                          '/Event/TPC/Physics/Pulses/S2XYPositionMercury',
                          '/Event/TpcHighGain/Physics/Pulses/Boundaries',
                          '/Event/TpcHighGain/Physics/Pulses/Parameters',
                          '/Event/TpcHighGain/Physics/Pulses/ChannelPulseParameters',
                          '/Event/TpcHighGain/Physics/Pulses/Classifications',
                          '/Event/TpcHighGain/Physics/Pulses/ChannelPulsePhotons',
                          '/Event/TpcHighGain/Physics/Pulses/Photons',
                          #'/Event/TpcHighGain/Physics/Pulses/S2XYPositionMercury',
                          '/Event/TpcLowGain/Physics/Pulses/Boundaries',
                          '/Event/TpcLowGain/Physics/Pulses/Parameters',
                          '/Event/TpcLowGain/Physics/Pulses/ChannelPulseParameters',
                          '/Event/TpcLowGain/Physics/Pulses/Classifications',
                          '/Event/TpcLowGain/Physics/Pulses/ChannelPulsePhotons',
                          '/Event/TpcLowGain/Physics/Pulses/Photons',
                          #'/Event/TpcLowGain/Physics/Pulses/S2XYPositionMercury',
                          '/Event/Physics/Pulses/HGLGMapping',
                          '/Event/Skin/Physics/Pulses/Boundaries',
                          '/Event/Skin/Physics/Pulses/Parameters',
                          '/Event/Skin/Physics/Pulses/ChannelPulseParameters',
                          '/Event/Skin/Physics/Pulses/Classifications',
                          '/Event/Skin/Physics/Pulses/ChannelPulsePhotons',
                          '/Event/Skin/Physics/Pulses/Photons',
                          '/Event/OuterHighGain/Physics/Pulses/Boundaries',
                          '/Event/OuterHighGain/Physics/Pulses/ODParameters',
                          '/Event/OuterHighGain/Physics/Pulses/ODChannelPulseParameters',
                          '/Event/OuterLowGain/Physics/Pulses/Boundaries',
                          '/Event/OuterLowGain/Physics/Pulses/ODParameters',
                          '/Event/OuterLowGain/Physics/Pulses/ODChannelPulseParameters',
                          '/Event/Physics/Interactions',
                          '/Event/Physics/MultipleScatterInteractions',
                          '/Event/Physics/Kr83mLikeInteractions',
                          '/Event/Physics/PileUpInteractions',
                          '/Event/Physics/OtherInteractions',
                          '/Event/TpcHighGain/SummedPods',
                          '/Event/TpcLowGain/SummedPods',
                          '/Event/Skin/SummedPods',
                          '/Event/OuterHighGain/SummedPods',
                          '/Event/OuterLowGain/SummedPods' ]
outStream.Output="SVC='RQConversionSvc' OPT='RECREATE'"
outStream.EvtConversionSvc="PersistencySvc"
app.OutStream = [ outStream ]

mcOutStream=RQModulesConf.RQ__RQOutputStream('RQMCWriter')
mcOutStream.OutputFile = mcOutputFile
mcOutStream.OptItemList = [ '/Event/DetectorMC' ]
mcOutStream.Output = "SVC='RQMCTruthConversionSvc' OPT='RECREATE'"
mcOutStream.EvtConversionSvc = "PersistencySvc"
app.OutStream += [ mcOutStream ]


# Configure LZap modules

from PhotonDetection import PhotonDetectionConf
from InteractionDetection import InteractionDetectionConf

################################################
# Common modules for all detectors

# Pod Calibrator
podCalibrator = PhotonDetectionConf.LZap__PodCalibrator('PodCalibrator')
podCalibrator.preBaselineSamples = 10
podCalibrator.postBaselineSamples = 10


################################################
# TPC High Gain modules
TPCHG = 'TPC High Gain'

TPCHGPodSummer = PhotonDetectionConf.LZap__PodSummer('TPCHGPodSummer')
TPCHGPodSummer.detector = TPCHG
TPCHGPodSummer.minSeparationNs = 3000

TPCHGDoGPulseFinder = PhotonDetectionConf.LZap__DoGPulseFinder('TPCHGDoGPulseFinder')
TPCHGDoGPulseFinder.detector = TPCHG

TPCHGChannelPulseParameterizer = PhotonDetectionConf.LZap__ChannelPulseParameterizer('TPCHGChannelPulseParameterizer')
TPCHGChannelPulseParameterizer.detector = TPCHG

TPCHGPulseParameterizer = PhotonDetectionConf.LZap__PulseParameterizer('TPCHGPulseParameterizer')
TPCHGPulseParameterizer.detector = TPCHG

TPCHGPulseClassifier = PhotonDetectionConf.LZap__PulseClassifierHADES('TPCHGPulseClassifier')
TPCHGPulseClassifier.detector = TPCHG

TPCHGPhotonCounter = PhotonDetectionConf.LZap__PhotonCounter('TPCHGPhotonCounter')
TPCHGPhotonCounter.detector = TPCHG
TPCHGPhotonCounter.constThreshold_phd = 0.0015

################################################
# TPC Low Gain modules
TPCLG = 'TPC Low Gain'

TPCLGPodSummer = PhotonDetectionConf.LZap__PodSummer('TPCLGPodSummer')
TPCLGPodSummer.detector = TPCLG
TPCLGPodSummer.minSeparationNs = 3000

TPCLGDoGPulseFinder = PhotonDetectionConf.LZap__DoGPulseFinder('TPCLGDoGPulseFinder')
TPCLGDoGPulseFinder.detector = TPCLG

TPCLGChannelPulseParameterizer = PhotonDetectionConf.LZap__ChannelPulseParameterizer('TPCLGChannelPulseParameterizer')
TPCLGChannelPulseParameterizer.detector = TPCLG

TPCLGPulseParameterizer = PhotonDetectionConf.LZap__PulseParameterizer('TPCLGPulseParameterizer')
TPCLGPulseParameterizer.detector = TPCLG

TPCLGPulseClassifier = PhotonDetectionConf.LZap__PulseClassifierHADES('TPCLGPulseClassifier')
TPCLGPulseClassifier.detector = TPCLG

TPCLGPhotonCounter = PhotonDetectionConf.LZap__PhotonCounter('TPCLGPhotonCounter')
TPCLGPhotonCounter.detector = TPCLG
TPCLGPhotonCounter.constThreshold_phd = 0.002

################################################
# Skin modules
Skin = 'Skin'

SkinPodSummer = PhotonDetectionConf.LZap__PodSummer('SkinPodSummer')
SkinPodSummer.detector = Skin
SkinPodSummer.minSeparationNs = 100

SkinDoGPulseFinder = PhotonDetectionConf.LZap__DoGPulseFinder('SkinDoGPulseFinder')
SkinDoGPulseFinder.detector = Skin

SkinChannelPulseParameterizer = PhotonDetectionConf.LZap__ChannelPulseParameterizer('SkinChannelPulseParameterizer')
SkinChannelPulseParameterizer.detector = Skin

SkinPulseParameterizer = PhotonDetectionConf.LZap__PulseParameterizer('SkinPulseParameterizer')
SkinPulseParameterizer.detector = Skin

SkinPulseClassifier = PhotonDetectionConf.LZap__PulseClassifierHADES('SkinPulseClassifier')
SkinPulseClassifier.detector = Skin

SkinPhotonCounter = PhotonDetectionConf.LZap__PhotonCounter('SkinPhotonCounter')
SkinPhotonCounter.detector = Skin
SkinPhotonCounter.constThreshold_phd = 0.0015

################################################
# OD High Gain modules
ODHG = 'Outer High Gain'

ODHGPodSummer = PhotonDetectionConf.LZap__PodSummer('ODHGPodSummer')
ODHGPodSummer.detector = ODHG
ODHGPodSummer.minSeparationNs = 3000

ODHGDoGPulseFinder = PhotonDetectionConf.LZap__DoGPulseFinder('ODHGDoGPulseFinder')
ODHGDoGPulseFinder.detector = ODHG

ODHGChannelPulseParameterizer = PhotonDetectionConf.LZap__ChannelPulseParameterizer('ODHGChannelPulseParameterizer')
ODHGChannelPulseParameterizer.detector = ODHG

ODHGChannelPulseParameterizerOD = PhotonDetectionConf.LZap__ODChannelPulseParameterizer('ODHGChannelPulseParameterizerOD')
ODHGChannelPulseParameterizerOD.detector = ODHG

ODHGPulseParameterizer = PhotonDetectionConf.LZap__PulseParameterizer('ODHGPulseParameterizer')
ODHGPulseParameterizer.detector = ODHG

ODHGPulseParameterizerOD = PhotonDetectionConf.LZap__ODPulseParameterizer('ODHGPulseParameterizerOD')
ODHGPulseParameterizerOD.detector = ODHG

################################################
# OD Low Gain modules
ODLG = 'Outer Low Gain'

ODLGPodSummer = PhotonDetectionConf.LZap__PodSummer('ODLGPodSummer')
ODLGPodSummer.detector = ODLG
ODLGPodSummer.minSeparationNs = 3000

ODLGDoGPulseFinder = PhotonDetectionConf.LZap__DoGPulseFinder('ODLGDoGPulseFinder')
ODLGDoGPulseFinder.detector = ODLG

ODLGChannelPulseParameterizer = PhotonDetectionConf.LZap__ChannelPulseParameterizer('ODLGChannelPulseParameterizer')
ODLGChannelPulseParameterizer.detector = ODLG

ODLGChannelPulseParameterizerOD = PhotonDetectionConf.LZap__ODChannelPulseParameterizer('ODLGChannelPulseParameterizerOD')
ODLGChannelPulseParameterizerOD.detector = ODLG

ODLGPulseParameterizer = PhotonDetectionConf.LZap__PulseParameterizer('ODLGPulseParameterizer')
ODLGPulseParameterizer.detector = ODLG

ODLGPulseParameterizerOD = PhotonDetectionConf.LZap__ODPulseParameterizer('ODLGPulseParameterizerOD')
ODLGPulseParameterizerOD.detector = ODLG

################################################
# Combined TPC modules

TPC = 'TPC'

HGLGMixer = PhotonDetectionConf.LZap__HGLGMixer()

TPCPulseClassifier = PhotonDetectionConf.LZap__PulseClassifierHADES('TPCPulseClassifier')
TPCPulseClassifier.detector = TPC

TPCPhotonCounter = PhotonDetectionConf.LZap__PhotonCounter('TPCPhotonCounter')
TPCPhotonCounter.detector = TPC
TPCPhotonCounter.constThreshold_phd = 0.0015

TPCGainMatcher = PhotonDetectionConf.PhotonDetection__GainMatcher()
TPCGainMatcher.HGdetector = TPCHG
TPCGainMatcher.LGdetector = TPCLG

TPCS2XY = InteractionDetectionConf.InteractionDetection__S2PositionReconstructorMercury()
TPCS2XY.jsonPathHG = '/cvmfs/lz.opensciencegrid.org/Physics/Components/LRF-01Jul19/LRF-mdc3-hg-indi.json'
TPCS2XY.jsonPathLG = '/cvmfs/lz.opensciencegrid.org/Physics/Components/LRF-01Jul19/LRF-mdc3-lg-indi.json'
TPCS2XY.Detector = TPC

interactionFinder       = InteractionDetectionConf.InteractionDetection__InteractionFinder()
xyzPositionCorrector    = InteractionDetectionConf.InteractionDetection__XYZPositionCorrector()
photonOverlapCorrector  = InteractionDetectionConf.InteractionDetection__PhotonOverlapCorrector()
pulseAreaCorrector      = InteractionDetectionConf.InteractionDetection__PulseAreaCorrector()

#################################################
# Define processing chain
commonModules = [ #rawEventCorrector,
                  podCalibrator ]
TPCHGModules = [ TPCHGPodSummer,
                 TPCHGDoGPulseFinder,
                 TPCHGChannelPulseParameterizer,
                 TPCHGPulseParameterizer,
                 TPCHGPulseClassifier,
                 TPCHGPhotonCounter ]
TPCLGModules = [ TPCLGPodSummer,
                 TPCLGDoGPulseFinder,
                 TPCLGChannelPulseParameterizer,
                 TPCLGPulseParameterizer,
                 TPCLGPulseClassifier,
                 TPCLGPhotonCounter ]
SkinModules  = [ SkinPodSummer,
                 SkinDoGPulseFinder,
                 SkinChannelPulseParameterizer,
                 SkinPulseParameterizer,
                 SkinPulseClassifier,
                 SkinPhotonCounter ]
ODHGModules  = [ ODHGPodSummer,
                 ODHGDoGPulseFinder,
                 ODHGChannelPulseParameterizer,
                 ODHGChannelPulseParameterizerOD,
                 ODHGPulseParameterizer,
                 ODHGPulseParameterizerOD ]
ODLGModules  = [ ODLGPodSummer,
                 ODLGDoGPulseFinder,
                 ODLGChannelPulseParameterizer,
                 ODLGChannelPulseParameterizerOD,
                 ODLGPulseParameterizer,
                 ODLGPulseParameterizerOD ]
TPCModules   = [ HGLGMixer,
                 TPCPulseClassifier,
                 TPCPhotonCounter,
                 TPCGainMatcher,
                 TPCS2XY,
                 interactionFinder ]
                 #xyzPositionCorrector,
                 #photonOverlapCorrector,
                 #pulseAreaCorrector ]
app.TopAlg = commonModules + TPCHGModules + TPCLGModules + TPCModules + SkinModules + ODHGModules + ODLGModules
