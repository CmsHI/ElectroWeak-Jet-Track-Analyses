### Configuration files for producing the final plots

- zJetPlot.sum.PRL.conf produces the paper plots for data
- zJetPlot.sum.PRL.plotTheory.mode10.conf produces the xjz theory comparison plot in the paper

- zJetPlot.sum.PRL.PPMC.Madgraph.plotTheoryPP.mode9.conf produces the pp only theory comparison plots
- zJetPlot.sum.PRL.plotTheoryHI.conf produces the PbPb only theory comparison plots

### Which configuration imports which ?

- zJetPlot.frag.conf is the mother configuration file
- zJetPlot.sum.PRL.conf imports zJetPlot.frag.conf
- zJetPlot.sum.PRL.plotTheory.conf imports zJetPlot.sum.PRL.conf

- zJetPlot.sum.PRL.PPMC.MadgraphPythia.plotTheoryPP.conf imports zJetPlot.sum.PRL.plotTheory.conf

- zJetPlot.sum.PRL.plotTheory.mode10.conf imports zJetPlot.sum.PRL.conf
- zJetPlot.sum.PRL.PPMC.Madgraph.plotTheoryPP.mode9.conf imports zJetPlot.sum.PRL.PPMC.MadgraphPythia.plotTheoryPP.conf
- zJetPlot.sum.PRL.plotTheoryHI.conf imports zJetPlot.sum.PRL.plotTheory.conf

