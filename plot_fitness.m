figure;

subplot(2,2,1);
hold on, grid on;
dst_pos = fit_raw_F1(:,2)+fit_raw_F1(:,3);
dst_neg = fit_raw_F1(:,2)-fit_raw_F1(:,3);
plot(fit_raw_F1(:,1),fit_raw_F1(:,2))
plot(fit_raw_F1(:,1),dst_pos,'r-')
plot(fit_raw_F1(:,1),dst_neg,'r-')
hold off;
title('RAW_{F1}')

subplot(2,2,2);
hold on, grid on;
dst_pos = fit_retina_F1(:,2)+fit_retina_F1(:,3);
dst_neg = fit_retina_F1(:,2)-fit_retina_F1(:,3);
plot(fit_raw_F1(:,1),fit_retina_F1(:,2))
plot(fit_raw_F1(:,1),dst_pos,'r-')
plot(fit_raw_F1(:,1),dst_neg,'r-')
hold off;
title('RETINA_{F1}')

subplot(2,2,3);
hold on, grid on;
dst_pos = fit_raw_F2(:,2)+fit_raw_F2(:,3);
dst_neg = fit_raw_F2(:,2)-fit_raw_F2(:,3);
plot(fit_raw_F2(:,1),fit_raw_F2(:,2))
plot(fit_raw_F2(:,1),dst_pos,'r-')
plot(fit_raw_F2(:,1),dst_neg,'r-')
hold off;
title('RAW_{F2}')

subplot(2,2,4);
hold on, grid on;
dst_pos = fit_retina_F2(:,2)+fit_retina_F2(:,3);
dst_neg = fit_retina_F2(:,2)-fit_retina_F2(:,3);
plot(fit_raw_F1(:,1),fit_retina_F2(:,2))
plot(fit_raw_F1(:,1),dst_pos,'r-')
plot(fit_raw_F1(:,1),dst_neg,'r-')
hold off;
title('RETINA_{F1}')