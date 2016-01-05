package com.rusefi.ui.config;

import com.rusefi.ConfigurationImage;
import com.rusefi.config.Field;
import com.rusefi.config.FieldType;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.Pair;
import com.rusefi.ui.util.JTextFieldWithWidth;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.nio.ByteBuffer;

public class ConfigField extends BaseConfigField {
    private final JTextField view = new JTextFieldWithWidth(200);

    public ConfigField(final Field field, String topLabel) {
        super(field);
        createUi(topLabel, view);
        requestInitialValue(field); // this is not in base constructor so that view is created by the time we invoke it

        MessagesCentral.getInstance().addListener(new MessagesCentral.MessageListener() {
            @Override
            public void onMessage(Class clazz, String message) {
                if (Field.isIntValueMessage(message) || Field.isFloatValueMessage(message)) {
                    Pair<Integer, ?> p = Field.parseResponse(message);
                    if (p != null && p.first == field.getOffset()) {
                        Object value = p.second;
                        setValue(value);
                    }
                }
            }
        });

        view.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_ENTER) {
                    sendValue(field, ConfigField.this.view.getText());
                }
            }
        });
    }

    private void setValue(Object value) {
        view.setEnabled(true);
        view.setText("" + value);
        onValueArrived();
    }

    @Override
    protected void loadValue(ConfigurationImage ci) {
        Number value = getValue(ci, field);
        setValue(value);
    }

    @NotNull
    public static Number getValue(ConfigurationImage ci, Field field) {
        Number value;
        ByteBuffer wrapped = getByteBuffer(ci, field);
        if (field.getType() == FieldType.INT) {
            value = wrapped.getInt();
        } else {
            value = wrapped.getFloat();
        }
        return value;
    }

    public static double getFloatValue(ConfigurationImage ci, Field field) {
        // this hack is trying to restore lost precision. It's a lame hack
        float value = getValue(ci, field).floatValue();
        return Double.parseDouble(String.format("%.4f", value));
    }

    public static int getIntValue(ConfigurationImage ci, Field field) {
        return getValue(ci, field).intValue();
    }
}