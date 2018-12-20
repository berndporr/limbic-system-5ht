# Limbic system simulator (5HT and DA system)

![alt tag](playground.png)

A simulator for reward based learning.

## Experiment

The experiment simulates patience for reward where the agent needs to wait in front
of the landmark. The rewards appears after 150 (100) time steps. If the agent approaches
the landmark too early it's placed back to the start position.

We investigate how a lack of serotonin (because of a DRN suppression) can be compensated
by SSRI, psychedelics and environmental changes.

## Behaviour based simulator

### Stimuli

 * Visual landmark (dark green and bright green objects)
 * Place fields around them
 * Reward signal at the landmark which can be assigned to one of the landmarks
 * Switching of the reward from one landmark to the other (reversal)
 * Reward delivery can be delayed for delayed reward paradigms

### Motor output
 * Two signals to either approach the dark green landmark or the bright one

### Requires
 * QT5
 * IIR1 libaray (http://www.berndporr.me.uk/iir/)

### Compile under Linux
 * Install the IIR filter library https://github.com/berndporr/iir1 or install it via ppa: "ppa:berndporr/usbdux" and  "apt-get install iir1-dev".
 * Install the other packages: "apt-get install qt5-qmake, g++, qtbase5-dev-tools, qtdeclarative5-dev-tools, qtbase5-dev, qt5-default"
 * qmake
 * make

### How to run?

There are two different modes: single run (-a) and statistical run (-b).

The option -p selects the different scenarios (see below) which cause a lack of
5HT and different options to fix it. With the -p option subdirectories are
created according to the scenario.

Do run stats of all scenarios type: `run_all_stats.sh`.

```
./limbic-system-simulator: command line options
 -a:            single food run
 -b:            statistics
 -f max:        max contacts to the landmark with food for stats
 -q:            quicktime export
 -n:            no graphical output
 -p scenario #: experimental scenario
Possible scenarios:
0 : Normal condition (/normal/)
1 : Reward shows up earlier (/normal_less_wait/)
2 : DRN is suppressed (/drn_suppress/)
3 : DRN is suppressed and less wait (/drn_suppress_less_wait/)
4 : DRN is suppressed and SSRI (/drn_suppress_ssri/)
5 : DRN is suppressed, SSRI and less wait (/drn_suppress_ssri_less_wait/)
6 : DRN is suppressed and 5HTR2 up (/drn_suppress_5ht2up/)
7 : DRN is suppressed, 5HTR2 up and less wait (/drn_suppress_5ht2up_less_wait/)
```

## Model checking

An abstract version in form of a prism model checker
is in the subdirectory `prism_model`.
