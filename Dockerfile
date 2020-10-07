FROM gcc:10
WORKDIR /app/
COPY ./LexicalAnalyzer.c ./
RUN gcc LexicalAnalyzer.c -o LexicalAnalyzer
RUN chmod +x LexicalAnalyzer