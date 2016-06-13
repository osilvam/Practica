function [y] = ChampionsPlot()
x = '../bin/champions.txt';
A = importdata(x, '\t');
g = A(:,1);
f = A(:,2);

figure
hold on

plot(g, f, 'DisplayName', 'Fitness')
xlabel('Generation')
ylabel('Fitness')
legend('show')
end