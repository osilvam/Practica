function [y] = ChampionsPlot()
x = '../bin/simulation_files/champions.txt';
A = importdata(x, '\t');
g = A.data(:,1);
f = A.data(:,3);

figure
hold on

plot(g, f, 'DisplayName', 'Fitness')
xlabel('Generation')
ylabel('Fitness')
legend('show')
end