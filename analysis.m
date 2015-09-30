% Inner Product Projcet
% Unoptimized vs. Optimized Analysis

x = [60 120 600 1200 6000 12000 60000 120000 600000 1200000 ...
    6000000 12000000 6000000 72000000 84000000 96000000 ...
    108000000 120000000];
y1 = [1.14 2.28 7.41 15.96 75.8282 151.656 775.386 ...
    1534.81 7681.46 16470 76125 153620 766890 924066 1.06534*10^6 ...
    1.22133*10^6 1.36841*10^6 1.53794*10^6];
y2 = [1.14207 1.14207 1.7 4 18 30 96 204 1461 2459 10089 21537 ...
    116924 125854 140120 160060 181162 199886];
plot(x, y1, ':', 'LineWidth', 3);
hold on
plot(x, y2,'--', 'LineWidth', 3);
hold off;
grid
xlabel('Array Size(# of elements)');
ylabel('Time (microseconds)');
title('Unoptimized vs. Optimized Dot Product')
legend('unoptimized', 'optimized')
