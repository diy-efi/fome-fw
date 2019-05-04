package com.rusefi.ui.etb;

import com.rusefi.ETBPane;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.etb.EtbTarget;
import com.rusefi.etb.StandardTestSequence;
import com.rusefi.etb.TestSequenceStep;
import com.rusefi.io.CommandQueue;

import javax.swing.*;

import java.util.Random;
import java.util.concurrent.atomic.AtomicInteger;

import static com.romraider.util.ThreadUtil.sleep;
import static com.rusefi.SensorLogger.getSecondsSinceFileStart;
import static com.rusefi.Timeouts.SECOND;
import static com.rusefi.etb.TestSequenceStep.count;
import static com.rusefi.ui.etb.DirectDrivePanel.CANCEL_DIRECT_DRIVE_COMMAND;
import static com.rusefi.ui.etb.EtbTestSequence.*;

/**
 * 3/2/2019
 * (c) Andrey Belomutskiy
 */
public class EtbMonteCarloSequence {
    private static final int TOTAL_CYCLES_COUNT = 300;
    private static final double DEFAULT_POSITION = 7;
    private final JButton button = new JButton("ETB I feel lucky!");
    private final static Random r = new Random();
    private int counter;

    private double bestResultSoFar = 75;

    public EtbMonteCarloSequence() {
        button.addActionListener(e -> {
            counter = 0;

            executor.execute(new Runnable() {
                @Override
                public void run() {
                    CommandQueue.getInstance().write(CANCEL_DIRECT_DRIVE_COMMAND);
                    sleep(3 * SECOND);
                    // 3000 data points at 10Hz should be 300 seconds worth of data
                    StandardTestSequence.metric.start(/* buffer size: */3000, /*period, ms: */ 100);

                    // start first cycle. At the end of the run it would decide if it wants to start from beginning again
                    executor.execute(() -> runRandomCycle());
                }
            });
        });
    }

    private void runRandomCycle() {
        final int offset = r.nextInt(100);
        final double pFactor = 1 + (r.nextInt(300) / 100.0);
        final double iFactor = r.nextInt(30) / 100.0;
        final double dFactor = r.nextInt(30) / 100.0;
        String stats = "mcstats:offset:" + offset +
                ":pFactor:" + pFactor +
                ":iFactor:" + iFactor +
                ":dFactor:" + dFactor;
        MessagesCentral.getInstance().postMessage(EtbMonteCarloSequence.class, stats);
        CommandQueue.getInstance().write("etbreset");
        CommandQueue.getInstance().write("set etb_o " + offset);
        CommandQueue.getInstance().write("set etb_p " + pFactor);
        CommandQueue.getInstance().write("set etb_i " + iFactor);
        CommandQueue.getInstance().write("set etb_d " + dFactor);

        MessagesCentral.getInstance().postMessage(EtbMonteCarloSequence.class,
                getSecondsSinceFileStart() + " running " + stats);

        TestSequenceStep firstStep = new EtbTarget(10 * SECOND, DEFAULT_POSITION, null, TestSequenceStep.Condition.YES);
        TestSequenceStep.Condition condition = new TestSequenceStep.Condition() {
            @Override
            public boolean shouldRunTask() {
                double currentValue = StandardTestSequence.metric.getStandardDeviation();
                boolean shouldRun = currentValue < bestResultSoFar;
                if (!shouldRun) {
                    MessagesCentral.getInstance().postMessage(EtbMonteCarloSequence.class,
                            "Two much error accumulated, aborting! " + currentValue + " > " + bestResultSoFar);

                }
                return shouldRun;
            }
        };

        AtomicInteger stepCounter = new AtomicInteger();
        AtomicInteger totalSteps = new AtomicInteger();

        Runnable onEachStep = () -> SwingUtilities.invokeLater(() -> {
            String state = stepCounter.incrementAndGet() + "/" + totalSteps.get();
            double value = StandardTestSequence.metric.getStandardDeviation();
            MessagesCentral.getInstance().postMessage(EtbMonteCarloSequence.class,"Running " + state + ", current=" + value);
        });

        TestSequenceStep last = StandardTestSequence.addSequence(firstStep, onEachStep, condition);
        last.addNext(new TestSequenceStep(5 * SECOND, EtbTarget.Condition.YES) {
            @Override
            protected void doJob() {
                double cycleResult = SensorCentral.getInstance().getValue(Sensor.ETB_CONTROL_QUALITY);
                if (cycleResult < bestResultSoFar) {
                    bestResultSoFar = cycleResult;
                    MessagesCentral.getInstance().postMessage(EtbMonteCarloSequence.class,
                            getSecondsSinceFileStart() + ":" + stats + ":new_record:" + bestResultSoFar);
                }
                MessagesCentral.getInstance().postMessage(EtbMonteCarloSequence.class,
                        getSecondsSinceFileStart() + ":" + stats + ":result:" + cycleResult);
                if (counter == TOTAL_CYCLES_COUNT) {
                    CommandQueue.getInstance().write(ETBPane.SET_ETB + 0);
                    MessagesCentral.getInstance().postMessage(EtbTestSequence.class, "ETB MC sequence done!");
                    return;
                }
                counter++;
                MessagesCentral.getInstance().postMessage(EtbTestSequence.class, "Starting " + counter + " of " + TOTAL_CYCLES_COUNT);
                runRandomCycle();
            }
        });
        totalSteps.set(count(firstStep));
        firstStep.execute(executor);
    }

    public JButton getButton() {
        return button;
    }
}
