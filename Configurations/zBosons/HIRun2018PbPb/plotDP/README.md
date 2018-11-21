Plotting configurations for Z->ee detector performance in 2018 PbPb data.

The file with suffix 'frag.conf' is a config fragment, does not produce a plot by itself. It is imported by other config files that produce a plot.

### Making Plots

The following creates and runs the shell script which runs plotting configurations

  ```bash
  cd $EWJTAHOME # go to directory where the repository is based
  ShellScripts/Plotting/createRunPlots.sh Configurations/zBosons/HIRun2018PbPb/plotDP/ 'plot*.conf'
  Configurations/zBosons/HIRun2018PbPb/plotDP//runPlots.sh # make the plots
  ```
